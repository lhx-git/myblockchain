
//
// Created by lhx on 2/10/22.
//

#include "blockserver/transaction/impl/transaction_manager_impl.h"
#include "blockserver/database/impl/block_generator_impl.h"
#include "blockserver/comm/client_proxy/comm_controller.h"
#include "blockserver/transaction/transaction_wrapper.h"
#include "blockserver/message_fomat/transaction_result.h"
#include "blockserver/comm/stub/comm_stub.h"
#include "common/complie_config.h"
#include "glog/logging.h"

TransactionManagerImpl::TransactionManagerImpl() {
    this->commModule = new COMM_MODULE();                  // init rpc first
    this->blockGenerator = new BlockGeneratorImpl();    // then init blk gen
    this->lastEpochFinishSignal = blockGenerator->getLatestSavedEpoch();
    epoch = this->lastEpochFinishSignal + 1;            // after this, we can get the valid epoch
    LOG(INFO) << "TransactionManagerImpl start with initial epoch=" << epoch << std::endl;
    this->currentEpochTrWrapper = std::make_unique<TransactionWrapper>(epoch);    // and init base on this
    this->nextEpochTrWrapper = std::make_unique<TransactionWrapper>(epoch + 1);
}

void TransactionManagerImpl::run() {
    while (true) {
        TransactionWrapper* trWrapper;
        // 获取所有的epoch = i 的交易, 并且发送给Coordinator
        while(trWrapper = commModule->getTransaction(epoch, TR_MGR_GET_BATCH_MAX_SIZE, TR_MGR_GET_BATCH_MIN_SIZE), trWrapper!= nullptr) {
            trWrapper->setEpoch(epoch);
            currentEpochTrWrapper->pushTransactionUncheck(*trWrapper);
            sendTransactionHandle(trWrapper);
        }
        //DLOG(INFO) << "get all epoch =  " << epoch << " transaction";
        // 获取1个epoch = 1 的交易, 并且发送给Coordinator
        if (trWrapper = commModule->getTransaction(epoch + 1, TR_MGR_GET_BATCH_MAX_SIZE, TR_MGR_GET_BATCH_MIN_SIZE), trWrapper!= nullptr) {
            trWrapper->setEpoch(epoch);
            nextEpochTrWrapper->pushTransactionUncheck(*trWrapper);
            sendTransactionHandle(trWrapper);
        }
        //DLOG(INFO) << "get 1 epoch = " << epoch + 1 << " transaction";
        //检查第i个epoch的交易 是否已经处理完成
        if (lastEpochFinishSignal == epoch) {
            uint32_t commit = 0, abort = 0;
            trWrapper = new TransactionWrapper(epoch + 1);
            DLOG(INFO) << "finishEpoch: " << this->lastEpochFinishSignal;
            for (auto tr: currentEpochTrWrapper->getTransactionList()) {
                DCHECK(this->lastEpochFinishSignal == tr->getEpoch());
                // 将上一个epoch abort的交易重新打包成下一个epoch的交易
                if(tr->getTransactionResult() == TransactionResult::ABORT) {
                    DLOG(INFO) << "ABORT: " << tr->getTransactionID();
                    // reset the transaction
                    tr->resetTransaction();   //tr->getEpoch() == 0 after reset
                    tr->setEpoch(epoch + 1);
                    // wrap the transaction, if size == max send it
                    trWrapper->pushTransaction(tr);
                    if(trWrapper->size() == TR_MGR_GET_BATCH_MAX_SIZE) {
                        // resend the transaction
                        sendTransactionHandle(trWrapper);
                        trWrapper = new TransactionWrapper(epoch + 1);
                    }
                    // add the transaction to monitor list
                    nextEpochTrWrapper->pushTransaction(tr);
                    abort++;
                    continue;
                }
                // TODO: consider delete tr at here?
                // rank them into different map
                if(tr->getTransactionResult() == TransactionResult::COMMIT) {
                    DLOG(INFO) << "COMMIT: " << tr->getTransactionID();
                    blockGenerator->addCommittedTransaction(tr);    // add to blk gen
                    commit++;
                    continue;
                }
                if(tr->getTransactionResult() == TransactionResult::ABORT_NO_RETRY) {
                    DLOG(INFO) << "ABORT_NO_RETRY: " << tr->getTransactionID();
                    // aborted tr also has been execute successfully
                    blockGenerator->addCommittedTransaction(tr);
                    abort++;
                    continue;
                }
            }
            // send the rest tr wrapper
            sendTransactionHandle(trWrapper);
            // generate next block
            if (!blockGenerator->generateBlock()) {
                LOG(INFO) << "Generate Block Failed! ";
                CHECK(false);
            }
            // summary
            LOG(INFO) << "Total, abort: " << abort << " commit: "<<commit;
            LOG(INFO) << "Finish with epoch id=" << lastEpochFinishSignal << std::endl;
            // move on to next epoch
            this->commModule->clearTransactionForEpoch(epoch);
            epoch += 1;
            // switch tr wrapper
//            delete this->currentEpochTrWrapper;
//            this->currentEpochTrWrapper = nextEpochTrWrapper;
//            this->nextEpochTrWrapper = new TransactionWrapper(epoch + 1);
            currentEpochTrWrapper.reset();
            currentEpochTrWrapper.reset(nextEpochTrWrapper.release());
            nextEpochTrWrapper.reset(new TransactionWrapper(epoch + 1));
        }
    }
}

TransactionManagerImpl::~TransactionManagerImpl() {
    delete (COMM_MODULE*) this->commModule;    //these need to be changed to impl as well.
    delete (BlockGeneratorImpl*) this->blockGenerator;
}
