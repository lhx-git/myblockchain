//
// Created by lhx on 2/9/22.
//

#include "blockserver/comm/client_proxy/comm_controller.h"
#include "blockserver/comm/client_proxy/user_collector.h"
#include "common/yaml_config.h"
#include "glog/logging.h"
#include "blockserver/transaction/mock/mock_transaction.h"
#include "blockserver/transaction/transaction_wrapper.h"
#include "blockserver/database/block_storage.h"
#include "blockserver/database/database.h"
#include "common/crypto_sign.h"
#include "common/sha256_hepler.h"
#include "comm.pb.h"


CommController::CommController() : ZMQServer("0.0.0.0", "7001", zmq::socket_type::pub), finishSignal(false),
    barrier(YAMLConfig::getInstance()->getBlockServerConst(), [&] () {
        LOG(INFO) << "All threads reached barrier, epoch: " << this->epoch << ", count: " << this->barrier.get_current_waiting();
        this->epoch++;
    }),
    processedTransaction([](Transaction* a, Transaction* b) { //小顶堆
        if (a->getEpoch() == b->getEpoch()) {
            return a->getTransactionID() > b->getTransactionID();
        }
        return a->getEpoch() > b->getEpoch();
    }){
    //1.初始化加密模块
    auto config = YAMLConfig::getInstance();
    epochServerSignHelper = new CryptoSign(config->getEpochServerConfig()->pubFilePath, config->getEpochServerConfig()->priFilePath);
    sha256Helper = new SHA256Helper();
    // ----this line below is the same as comm_stub----
    BlockStorage* blockStorage = BlockStorage::getInstance();
    // blockStorage->initFunc();
    // init block storage and get epoch
    epoch = blockStorage->getLatestSavedEpoch() + 1;
    // check if epoch == global latest epoch
    // pull blk from other server
    // start db
    VersionedDB::getDBInstance();
    // update write set (only committed tr s)
    // save block
    // ----this line above is same as comm_stub----
    //0. init
    //创建一个user collector
    userCollector = new UserCollector();
    //注册回调函数
    userCollector->newTransactionHandle = [this](const std::vector<Transaction*>& transaction, const std::string& proof) {
        // enqueue data first, then enqueue metadata.
        for(auto& data: transaction) {
            this->pendingTransactions.enqueue(data);
        }
        // this->pendingTransactions.enqueue_bulk(transaction.data(), transaction.size());
        this->pendingTransactionsProof.enqueue(proof);
    };
    userCollector->lastEpochSizeHandle = [this](uint32_t _epoch, size_t size) {
        epochSizeMap[_epoch] = size;
    };
    // 1. bring up userCollector
    // 收集用户发来的交易
    userCollectorThread = new std::thread(&UserCollector::run, userCollector);
    // 2. bring up local receiver (remote sender)
    // 将本地交易广播给其他节点
    localReceiverThread = new std::thread(&CommController::receiveFromLocalServer, this);
    // 收集其他节点发来的交易
    for (auto ip : config->getBlockServerIPs()) {
        if (ip == config->getLocalBlockServerIP()) {
            continue;
        }
        remoteReceiverThreads.push_back(new std::thread(&CommController::receiveFromOtherServer,
                                                        this, new ZMQClient(ip, "7001", zmq::socket_type::sub)));
    }

}

CommController::~CommController() {
    delete epochServerSignHelper;
    delete sha256Helper;
    delete userCollector;
    finishSignal = true;
    userCollectorThread->join();
    localReceiverThread->join();
    for(std::thread* remoteReceiverThread: remoteReceiverThreads) {
        remoteReceiverThread->join();
        delete remoteReceiverThread;
    }

}

void CommController::receiveFromLocalServer() {
    std::vector<Transaction*> transactions;
    std::string responseRaw;
    comm::EpochResponse response;
    comm::ProxyBroadcast transactionSetMulti;

    while (!finishSignal) {
        //1.获取一批交易 以及对应的证明
        pendingTransactionsProof.wait_dequeue(responseRaw);
        response.ParseFromString(responseRaw);
        transactions.resize(response.request().request_batch_size());
        pendingTransactions.wait_dequeue_bulk(transactions.data(), response.request().request_batch_size());

        //2.检查是否本地该epoch的交易已经收集完全, 广播给其他blockserver
        if (response.epoch() > epoch) {
            //广播给其他blockserver
            transactionSetMulti.set_epoch(epoch);
            sendReply(transactionSetMulti.SerializeAsString());

            transactionSetMulti.Clear();
            // wait until all trs in this epoch collected. --local
            wait();
            // after this epoch += 1, can safely enqueue
        }
        //3.产生一个交易集合
        CHECK(response.epoch() == epoch);
        auto* transactionSet = transactionSetMulti.add_set();
        transactionSet->set_proof(responseRaw);
        for (auto transaction : transactions) {
            CHECK(transaction->getEpoch() == epoch);

            std::string* transactionRaw = transactionSet->add_transaction();
            CHECK(transaction->serializeToString(transactionRaw));
            // 3.1 将交易加入到broadcastTransactions中,
            // generate transaction id
            sha256Helper->hash(response.signature());
            sha256Helper->hash(transaction->getTransactionPayload()->digest());
            std::string digest;
            sha256Helper->execute(&digest);
            transaction->setTransactionID(*reinterpret_cast<uint32_t*>(digest.data()));
            // enqueue transaction
            broadcastTransactions.enqueue(transaction);
        }
        if(!verifyBatchHash(transactionSet, response)) {
            LOG(ERROR) << "local server recv thread: signature error!";
        }
    }
}

bool CommController::verifyBatchHash(const comm::TransactionsWithProof *transactionSet,
                                     const comm::EpochResponse &response) {
    if(transactionSet->transaction().size() != response.request().request_batch_size()) {
        DLOG(ERROR) << "pre-check: batch size error!";
        return false;
    }
    //交易完整性验证
    sha256Helper->reset();
    for(const auto& transactionRaw: transactionSet->transaction()) {
        MockTransaction transaction;
        transaction.deserializeFromString(transactionRaw);
        sha256Helper->hash(transaction.getTransactionPayload()->digest());
    }
    std::string digest;
    sha256Helper->execute(&digest);
    if(response.request().batch_hash() != digest) {
        DLOG(ERROR) << "pre-check: batch hash error!";
        return false;
    }

    // 验证签名  epoch + request_batch_hash + nonce : message
    if(!epochServerSignHelper->rsaDecrypt(std::to_string(response.epoch()) +
                                          response.request().batch_hash() +
                                          std::to_string(response.nonce()), response.signature()))
        return false;
    return true;
}

void CommController::receiveFromOtherServer(ZMQClient *client) {
    while(!finishSignal) {
        std::string messageRaw;
        client->getReply(messageRaw);

        comm::ProxyBroadcast transactionSetMulti;
        transactionSetMulti.ParseFromString(messageRaw);

        for(const auto& transactionSet: transactionSetMulti.set()) {
            // verify batch hash
            comm::EpochResponse response;
            response.ParseFromString(transactionSet.proof());
            if(!verifyBatchHash(&transactionSet, response)) {
                LOG(ERROR) << "remote server recv thread: signature error!";
            }
            for (const auto &transactionRaw :transactionSet.transaction()) {
                auto *transaction = new MockTransaction;
                // deserialize transaction here
                CHECK(transaction->deserializeFromString(transactionRaw));
                // add to broadcast queue
                CHECK(transaction->getEpoch() == epoch);
                // generate transaction id
                sha256Helper->hash(response.signature());
                sha256Helper->hash(transaction->getTransactionPayload()->digest());
                std::string digest;
                sha256Helper->execute(&digest);
                transaction->setTransactionID(*reinterpret_cast<uint32_t*>(digest.data()));
                // enqueue transaction
                broadcastTransactions.enqueue(transaction);
            }
        }
        wait();
    }
    delete client;
}

void CommController::transferTransaction() {
    // this func is a adaptor to processedTransaction, because concurrent queue does not support front() func.
    // when func return, make sure that processedTransaction has at least 1 tr
    Transaction* transaction;
    if(processedTransaction.empty()) {
        broadcastTransactions.wait_dequeue(transaction);
        processedTransaction.push(transaction);
    }
    while (broadcastTransactions.try_dequeue(transaction)) {
        processedTransaction.push(transaction);
    }
}

uint32_t currentEpoch = 0;
TransactionWrapper *CommController::getTransaction(uint32_t _epoch, uint32_t maxSize, uint32_t minSize) {
    DCHECK(_epoch <= this->epoch);
    // 0. check if we already gave all trs in _epoch
    if(!processedTransaction.empty()) {
        if(processedTransaction.top()->getEpoch() != _epoch) {
            DCHECK(processedTransaction.top()->getEpoch() == _epoch + 1 ||
                   processedTransaction.top()->getEpoch() == _epoch + 2);
            return nullptr;
        }
    }
    // 1. all trs in _epoch has not finished
    // 2. all trs in _epoch has finished, but we didnt received any _epoch+1 trs
    // for both 1 and 2, the processedTransaction queue must be empty,
    // so pull at least one transaction, check its epoch
    DCHECK(processedTransaction.empty() || processedTransaction.top()->getEpoch() == _epoch);
    // we must wait for a transaction to fill in processedTransaction queue
    transferTransaction();
    // check condition 2
    if(processedTransaction.top()->getEpoch() != _epoch) {
        DCHECK(processedTransaction.top()->getEpoch() == _epoch + 1);
        return nullptr;
    }
    // deal with condition 1
    auto* trWrapper = new TransactionWrapper(_epoch);
    // 1.1 we have emptied processedTransaction queue but still not / just collect all trs
    // 1.2 we have collected all trs in _epoch and queue is not empty
    // for 1.1
    // 1.1.1 greater than minSize, less than maxSize, return
    // 1.1.2 greater than maxSize, return
    // 1.1.3 less than minSize, block wait
    // the loop condition is 1.1.2
    while(trWrapper->size() < maxSize) {
        Transaction* transaction = processedTransaction.top();
        // 1.2
        if(transaction->getEpoch() != _epoch) {
            break;
        }
        CHECK(transaction->getEpoch() >= currentEpoch ? (currentEpoch = transaction->getEpoch(), true) : false);
        processedTransaction.pop();
        CHECK(trWrapper->pushTransaction(transaction));
        transactionMap[this->epoch].push_back(transaction);
        if(processedTransaction.empty()) {
            // 1.1.1
            if(trWrapper->size() > minSize) {
                break;
            }
            // 1.1.3
            transferTransaction();
        }
    }
    return trWrapper;
}

void CommController::clearTransactionForEpoch(uint32_t _epoch) {
    DCHECK(_epoch < this->epoch);
    std::vector<Transaction*> previousEpochTransaction = transactionMap[_epoch];
    for (auto tr: previousEpochTransaction) {
        if(tr->getEpoch() == _epoch) {
            delete tr;
            continue;
        }
        transactionMap[tr->getEpoch()].push_back(tr);
    }
    transactionMap.erase(_epoch);
}

void CommController::wait() {
    // re-enter protection
    // single server, epoch++ in callback func
    barrier.await();
}