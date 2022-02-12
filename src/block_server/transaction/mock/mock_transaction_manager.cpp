//
// Created by lhx on 1/18/22.
//

#include "blockserver/transaction/mock/mock_transaction_manager.h"
#include "blockserver/transaction/mock/mock_transaction.h"
#include "blockserver/transaction/transaction.h"
#include "blockserver/transaction/transaction_wrapper.h"
#include "thread"
#include "tpc-c.pb.h"
#include "glog/logging.h"

namespace mock {
    void generatePaload(TransactionPayload* transactionPayload, std::string key, int value) {
        transactionPayload->set_header("crdt_gcounter");
        CRDT_GCOUNTER_PAYLOAD crdtGcounterPayload;
        crdtGcounterPayload.set_key(key);
        crdtGcounterPayload.set_value(value);
        transactionPayload->set_payload(crdtGcounterPayload.SerializeAsString());
    }
}

void MockTransactionManager::run() {
    int32_t epoch = 0;//重要, epoch号从0开始, 这与transactionmanager中的 lastfinishedepochsignal匹配, Ariacoordinator的epoch也从0开始
    uint32_t tid = 1;//重要, 交易id要至少从1开始,从0开始会与保留表中的逻辑冲突
    std::vector<Transaction*> transactionList;
    while (true) {
        if (epoch == 0) {
            for (int i = 0; i < 5; i++) {
                TransactionWrapper* transactionWrapper = new TransactionWrapper(epoch);
                Transaction* transaction1 = new MockTransaction(epoch, tid);
                tid++;
                mock::generatePaload(transaction1->getTransactionPayload(), "lhx", 1);
                transactionWrapper->pushTransaction(transaction1);
                transactionList.push_back(transaction1);
                Transaction* transaction2 = new MockTransaction(epoch, tid);
                tid++;
                mock::generatePaload(transaction2->getTransactionPayload(), "lhx", 1);
                transactionWrapper->pushTransaction(transaction2);
                transactionList.push_back(transaction2);
                sendTransactionHandle(transactionWrapper);
            }
            TransactionWrapper* transactionWrapper1 = new TransactionWrapper(epoch + 1);
            Transaction* transaction = new MockTransaction(epoch + 1, tid);
            tid++;
            mock::generatePaload(transaction->getTransactionPayload(), "chw", 1);
            transactionWrapper1->pushTransaction(transaction);
            transactionList.push_back(transaction);
            sendTransactionHandle(transactionWrapper1);
        }

        if (epoch == 1) {
            for (int i = 0; i < 5; i++) {
                TransactionWrapper* transactionWrapper = new TransactionWrapper(epoch);
                Transaction* transaction1 = new MockTransaction(epoch, tid);
                tid++;
                mock::generatePaload(transaction1->getTransactionPayload(), "chw", 1);
                transactionWrapper->pushTransaction(transaction1);
                transactionList.push_back(transaction1);
                Transaction* transaction2 = new MockTransaction(epoch, tid);
                tid++;
                mock::generatePaload(transaction2->getTransactionPayload(), "chw", 1);
                transactionWrapper->pushTransaction(transaction2);
                transactionList.push_back(transaction2);
                sendTransactionHandle(transactionWrapper);
            }

            TransactionWrapper* transactionWrapper2 = new TransactionWrapper(epoch + 1);
            Transaction* transaction2 = new MockTransaction(epoch + 1, tid);
            tid++;
            mock::generatePaload(transaction2->getTransactionPayload(), "chw", 1);
            transactionWrapper2->pushTransaction(transaction2);
            sendTransactionHandle(transactionWrapper2);
        }

        while (lastEpochFinishSignal.load() < epoch) {
            std::this_thread::yield();
        }
        if (this->lastEpochFinishSignal <= epoch) {
            DCHECK(this->lastEpochFinishSignal == epoch);
            uint32_t commit = 0, abort = 0;
            std::vector<Transaction*> abortedList;
            for (auto tr: transactionList) {
                if (this->lastEpochFinishSignal < tr->getEpoch()) {
                    abortedList.push_back(tr);
                    continue;
                }
                // DLOG(INFO) << this->lastEpochFinishSignal << "a: " << tr->getEpoch();
                DCHECK(this->lastEpochFinishSignal == tr->getEpoch());
                // add last epoch transaction to current epoch
                if(tr->getTransactionResult() == TransactionResult::ABORT) {
                    DLOG(INFO) << "ABORT: " << tr->getTransactionID();
                    tr->resetTransaction();
                    tr->setEpoch(epoch + 1);   //tr->getEpoch() == 0
                    trWrapper = new TransactionWrapper(epoch + 1);
                    trWrapper->pushTransaction(tr);
                    sendTransactionHandle(trWrapper);   //resend the transaction
                    abortedList.push_back(tr);
                    abort++;
                    continue;
                }
                if(tr->getTransactionResult() == TransactionResult::COMMIT) {
                    DLOG(INFO) << "COMMIT: " << tr->getTransactionID();
                    // delete (MockTransaction*) tr;
                    commit++;
                    continue;
                }
                if(tr->getTransactionResult() == TransactionResult::ABORT_NO_RETRY) {
                    DLOG(INFO) << "ABORT_NO_RETRY: " << tr->getTransactionID();
                    commit++;
                    continue;
                }
            }
            transactionList = abortedList;
            LOG(INFO) << "Total, abort: " << abort << " commit: "<<commit;
            LOG(INFO) << "Finish with epoch id=" << lastEpochFinishSignal << std::endl;
            epoch++;
        }
    }
}

MockTransactionManager::~MockTransactionManager() {

}

