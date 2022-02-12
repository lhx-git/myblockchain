//
// Created by lhx on 1/14/22.
//

#include "glog/logging.h"
#include "blockserver/worker/AriaWorker.h"
#include "blockserver/message_fomat/worker_state.h"
#include "blockserver/message_fomat/aria_global_state.h"
#include "blockserver/worker/impl/transaction_executor_impl.h"
#include "blockserver/message_fomat/workload.h"
#include <memory>
AriaWorker::AriaWorker(WorkerInstance *self) : Worker(self) {
    self->workerState.store(static_cast<uint32_t>(WorkerState::READY));
    transactionExecutor = std::make_shared<TransactionExecutorImpl>();
}

AriaWorker::~AriaWorker()  {
    LOG(INFO) << "worker destroy id:"<< _self->workerID << std::endl;
}

void AriaWorker::run() {
    DLOG(INFO) << "run, worker id = " << _self->workerID << std::endl;
    //worker被创建时，必定为ASSIGNED状态
    setWorkerState(WorkerState::ASSIGNED);
    while (true) {
        //
        if (!waitForSignal(AriaGlobalState::Aria_READ)) {
            setWorkerState(WorkerState::EXITED);
            return ;
        }
        //执行交易
        transactionExecutor->executeList(_self->workload->transactionList);
        DLOG(INFO) << "worker id = " << _self->workerID  << " has finished read, epoch: " << _self->epochChan << std::endl;
        setWorkerState(WorkerState::FINISH_READ);

        if (!waitForSignal(AriaGlobalState::Aria_COMMIT)) {
            setWorkerState(WorkerState::EXITED);
            return ;
        }
        //提交事物
        transactionExecutor->commitList(_self->workload->transactionList);
        DLOG(INFO) << "worker id = " << _self->workerID  << " has finished commit, epoch: " << _self->epochChan << std::endl;
        setWorkerState(WorkerState::READY);
    }
}

void AriaWorker::setWorkerState(WorkerState state) {
    std::unique_lock<std::mutex> consumeLock(_self->mutex);
    _self->workerState.store(static_cast<uint32_t>(state));
    _self->notifyPruduce();
}

bool AriaWorker::waitForSignal(AriaGlobalState state) {
    std::unique_lock<std::mutex> consumeLock(_self->mutex);
    uint32_t signal;
    while (signal = _self->ariaGlobalState.load(), signal != static_cast<uint32_t>(state) && signal != static_cast<uint32_t>(AriaGlobalState::EXIT)) {
        _self->consume.wait(consumeLock);
    }
    if (signal == static_cast<uint32_t>(AriaGlobalState::EXIT)) {
        DLOG(INFO) << "worker id = " << _self->workerID  << " recv exit signal";
        // important! can not send signal here, deadlock, such as: setWorkerState(WorkerState::EXITED);
        return false;
    }
    return true;
}