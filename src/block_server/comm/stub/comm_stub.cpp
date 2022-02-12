//
// Created by peng on 2021/1/22.
//

#include "blockserver/comm/stub/comm_stub.h"
#include "blockserver/transaction/transaction_wrapper.h"

#include "glog/logging.h"
#include "transaction.pb.h"

#include "blockserver/transaction/mock/mock_transaction.h"
#include "blockserver/database/block_storage.h"
#include "blockserver/database/database.h"

TransactionWrapper* CommStub::getTransaction(uint32_t _epoch, uint32_t maxSize, uint32_t minSize) {
    if (_epoch < this->epoch) {
        DLOG(INFO) << "trm 's epoch = " << _epoch << "commStub 's epoch = " << epoch << std::endl;
        return nullptr;
    }
    auto* trWrapper = new TransactionWrapper(this->epoch);
    while(this->epochTrSentNum != EPOCH_MAX_TR_NUM && trWrapper->size() < minSize) {
        //----start create tr
        Transaction* transaction = new MockTransaction;
        transaction->setEpoch(this->epoch);
        transaction->setTransactionID(tidNonce++);

        TransactionPayload* payload = transaction->getTransactionPayload();
       /* ycsbPayload.Clear();

        for (int i=0; i<readSize; i++) {
            *ycsbPayload.add_reads() = std::to_string(randomHelper.uniform_dist(0,200000));
        }
        for (int i=0; i<writeSize; i++) {
            *ycsbPayload.add_update() = std::to_string(randomHelper.uniform_dist(0,200000));
        }*/

        crdtGcounterPayload.Clear();
        crdtGcounterPayload.set_key("lhx");
        crdtGcounterPayload.set_value(1);
        //todo: lhx modify here
        //payload->set_header("create_data");
        payload->set_header("crdt_gcounter");
        payload->set_payload(crdtGcounterPayload.SerializeAsString());
        payload->set_nonce(time(nullptr) + (uint64_t (tidNonce)<<32));
        //---- end create tr

        trWrapper->pushTransaction(transaction);

        transactionMap[this->epoch].push_back(transaction);
        this->epochTrSentNum += 1;
    }
    if (trWrapper->getTransactionList().size() != 0) {
        DLOG(INFO) << "send transaction epoch = : " << epoch << std::endl;
    }
    if (this->epochTrSentNum == EPOCH_MAX_TR_NUM) {
        epochTrSentNum = 0;
        this->epoch += 1;
    }
    return trWrapper;
}

void CommStub::clearTransactionForEpoch(uint32_t _epoch) {
    if(_epoch >= this->epoch)
        return;
    std::vector<Transaction*> previousEpochTransaction = transactionMap[_epoch];
    for (auto tr: previousEpochTransaction) {
        if(tr->getEpoch() == _epoch) {
            delete (MockTransaction*) tr;
            continue;
        }
        DCHECK(this->epoch == tr->getEpoch() || this->epoch == tr->getEpoch() + 1);
        transactionMap[tr->getEpoch()].push_back(tr);
    }
    transactionMap.erase(_epoch);
}

CommStub::CommStub() {
    BlockStorage* blockStorage = BlockStorage::getInstance();
    // blockStorage->initFunc();
    // init epoch
    epoch = blockStorage->getLatestSavedEpoch() + 1;
    // check if epoch == global latest epoch
    // pull blk from other server
    // start db
    VersionedDB::getDBInstance();
    // update write set (only committed tr s)
    // save block
}
