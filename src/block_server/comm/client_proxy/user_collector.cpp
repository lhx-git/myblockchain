
//
// Created by lhx on 2/9/22.
//

#include "blockserver/comm/client_proxy/user_collector.h"
#include "blockserver/transaction/mock/mock_transaction.h"
#include "common/yaml_config.h"
#include "common/crypto_sign.h"
#include "common/sha256_hepler.h"
#include "common/complie_config.h"
#include "comm.pb.h"
#include "glog/logging.h"

UserCollector::UserCollector() : ZMQServer("0.0.0.0", "5001"), finishSignal(false) {
    //5001端口提供收集user request服务
    collector = new std::thread(&UserCollector::collectRequest, this);
    //epoch服务器在6002端口提供服务
    epochServerClient = new ZMQClient(YAMLConfig::getInstance()->getEpochServerIPs()[0], "6002");
}

void UserCollector::collectRequest() {
    while (!finishSignal) {
        auto* message = new std::string();
        getRequest(*message);
        sendReply();//发送一个空字符串
        requestBuffer.enqueue(message);
    }
}

UserCollector::~UserCollector() {
    finishSignal = true;
    collector->join();
    delete collector;
    //清空requestBuffer
    std::string* message = nullptr;
    while (requestBuffer.try_dequeue(message)) {
        delete message;
    }
    delete epochServerClient;
}

void UserCollector::run() {
    auto blockServerConfig = YAMLConfig::getInstance()->getBlockServerConfig(YAMLConfig::getInstance()->getLocalBlockServerIP());
    CryptoSign userSignHelper(blockServerConfig->usrPubFilePath, blockServerConfig->usrPriFilePath);// 要与user的公私钥一致
    SHA256Helper sha256Helper;
    std::vector<std::string*> items;
    items.resize(EPOCH_SIZE_APPROX);
    while (true) {
        if (size_t itemSize = requestBuffer.wait_dequeue_bulk_timed(items.data(), EPOCH_SIZE_APPROX, DEQUEUE_TIMEOUT_US)) {
            //1.迭代items, 从items[]中反序列化 并 解析payload
            std::vector<Transaction*> transactionList;
            for (int i = 0; i < itemSize; i++) {
                //1.1 反序列化
                comm::UserRequest userRequest;
                userRequest.ParseFromString(*items[i]);
                delete items[i];
                //1.2 验签
                if (!userSignHelper.rsaDecrypt(userRequest.payload(), userRequest.digest())) {
                    CHECK(false);
                }
                //1.3 将签名加入 hash helper, 用于构造构造batch签名
                if (!sha256Helper.hash(userRequest.digest())) {
                    CHECK(false);
                }
                //1.4 构造交易
                MockTransaction *mockTransaction = new MockTransaction;
                TransactionPayload* transactionPayload = mockTransaction->getTransactionPayload();
                transactionPayload->ParseFromString(userRequest.payload());
                transactionPayload->set_digest(userRequest.digest());
                //1.5 交易加入transactionList
                transactionList.push_back(mockTransaction);
            }
            // 2. 给epoch服务器发送请求
            comm::EpochRequest request;
            // 2.1---1.3 由于我们已经遍历了交易列表,现在我们可以生成batch digest.
            std::string digest;
            sha256Helper.execute(&digest);
            // 2.2 构造request
            request.set_batch_hash(digest);
            request.set_request_batch_size(itemSize);
            // 2.3 发送请求,并且等待epoch服务器的Reply
            epochServerClient->sendRequest(request.SerializeAsString());
            // debug: check if current save epoch >= the epoch received from block server, error if true.
            // this could cause bug, because reading is NOT thread safe!
            std::string responseRaw;
            epochServerClient->getReply(responseRaw);
            comm::EpochResponse response;
            response.ParseFromString(responseRaw);
            // 2.4----1.4 附加epoch号
            for (Transaction* transaction: transactionList) {
                transaction->setEpoch(response.epoch());
            }
            // 3. 调用 handle, 发送交易列表, 证明 和 batch大小
            newTransactionHandle(transactionList, responseRaw);
            // DLOG(INFO) << "Call lastEpochSizeHandle, last epoch: " << response.epoch_number()-1 <<", size: " << 0;
            lastEpochSizeHandle(response.epoch() - 1, 0);
        }
    }
}