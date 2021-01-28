//
// Created by lhx on 1/21/21.
//

#include "block_server/coordinator/aria_coordinator.h"
#include "block_server/message_format/config.h"
#include "block_server/worker/aria_worker.h"
#include "glog/logging.h"
#include <memory>
#include "block_server/transaction/transaction_wrapper.h"

[[noreturn]] void AriaCoordinator::run() {
    while (true){
        std::shared_ptr<Workload> workload;
        bool isCurrentEpochEmpty = false;
        while (getWorkload(isCurrentEpochEmpty,workload),!isCurrentEpochEmpty){
            deployWorker(std::move(workload));
        }
        LOG(INFO)<< "start aria read";
        ariaGlobalState = AriaGlobalState::Aria_READ;
        broadToWorkers(ariaGlobalState);
        sycnAllWorkers(WorkerState::FINISH_READ);
        ariaGlobalState = AriaGlobalState::Aria_COMMIT;
        LOG(INFO)<< "start aria commit";
        broadToWorkers(ariaGlobalState);
        sycnAllWorkers(WorkerState::FINISH_COMMIT);
        LOG(INFO)<< "finish aria commit";
        setFinishedWorkerState(WorkerState::READY);
        this->epochChan++;
        produce.notify_one();
    }


}

void AriaCoordinator::createWorker(int32_t workerID) {
    auto worker = new WorkerInstance;
    workers[workerID++] = worker;
    worker->ariaGlobalState = AriaGlobalState::START;
    worker->workerState = WorkerState::READY;
    worker->workerID = workerID;
    worker->epoch = epochChan;
    worker->instance = std::make_shared<AriaWorker>(worker);
    worker->thread = std::make_shared<std::thread>(&Worker::run,worker->instance.get());
     // ploymorphism
}

void AriaCoordinator::broadToWorkers(AriaGlobalState state) {
    for(auto worker : workers)
    {
        worker.second->ariaGlobalState = this->ariaGlobalState;
        worker.second->consume.notify_one();
    }
}

void AriaCoordinator::sycnAllWorkers(WorkerState workerState) {
    for(auto worker : workers){
        std::unique_lock<std::mutex> produceLock(worker.second->mutex);
        if(worker.second->workerState == workerState)
            continue;
        worker.second->produce.wait(produceLock);

    }
}

void AriaCoordinator::setWorkerState(int32_t workerID, WorkerState workerState) {
    workers[workerID]->workerState = workerState;
}

WorkerState AriaCoordinator::getWorkerState(int32_t workerID) {
    return workers[workerID]->workerState;
}


uint32_t AriaCoordinator::addTransaction(std::shared_ptr<TransactionWrapper> trWrapper) {
    int32_t BUFFER_MAX_SIZE = 5;
    std::unique_lock<std::mutex> lock(transactionBufferMutex);

    produce.wait(lock,[this,trWrapper,BUFFER_MAX_SIZE]()->bool {
        return transactionBuffer.size()+trWrapper->size() <= BUFFER_MAX_SIZE;
    });

            for (auto tr:trWrapper->getTransactionList()) {
                transactionBuffer.push(tr);
            }
            LOG(INFO)<<"add a trWrapper";
            consume.notify_one();
            return 1;

}

bool AriaCoordinator::getWorkload(bool& isCurrentEpochEmpty,std::shared_ptr<Workload>& workload) {
    int32_t MIN_WORK_LOAD_SIZE = 2;
    int32_t MAX_WORK_LOAD_SIZE = 3;
    std::unique_lock<std::mutex> lock(transactionBufferMutex);
    consume.wait(lock,[this,MIN_WORK_LOAD_SIZE,workload]()->bool {
        return transactionBuffer.size() >= MIN_WORK_LOAD_SIZE;
    });

    if(transactionBuffer.front()->getEpoch()!=epochChan){
        isCurrentEpochEmpty =true;
        return false;
    }
    LOG(INFO)<<"create a workload  : " << epochChan;
    if(this->transactionBuffer.size() >= MAX_WORK_LOAD_SIZE)
    {
        workload = std::make_shared<Workload>();
        workload->setEpochNumber(this->epochChan);
        for(int i =0; i < MAX_WORK_LOAD_SIZE ;i++){
            if(transactionBuffer.front()->getEpoch()!=epochChan)
                break;
            workload->pushTransaction(transactionBuffer.front());
            transactionBuffer.pop();
        }
        produce.notify_one();
        return true;
    }

    workload = std::make_shared<Workload>();
    workload->setEpochNumber(this->epochChan);
    for(int i =0; i < MIN_WORK_LOAD_SIZE ;i++){
        if(transactionBuffer.front()->getEpoch()!=epochChan)
            break;
        workload->pushTransaction(transactionBuffer.front());
        transactionBuffer.pop();
    }
    produce.notify_one();
    return true;
}

bool AriaCoordinator::deployWorker(std::shared_ptr<Workload> workload) {
    //TODO just workload is not empty
        for(auto worker :workers ){
            if(worker.second->workerState == WorkerState::READY)
            {
                worker.second->workload = workload;
                worker.second->workerState = WorkerState::ASSIGNED;
                LOG(INFO)<< "deploy a workload to worker:  "<<worker.second->workerID;
                return true;
            }
        }
        auto worker = new WorkerInstance;
        workers[workerID++] = worker;
        worker->ariaGlobalState = AriaGlobalState::START;
        worker->workerState = WorkerState::ASSIGNED;
        worker->workerID = workerID;
        worker->workload = workload;
        worker->epoch = epochChan;
        worker->instance = std::make_shared<AriaWorker>(worker);
        worker->thread = std::make_shared<std::thread>(&Worker::run,worker->instance.get());
        LOG(INFO)<< "deploy a workload to worker:  "<<worker->workerID;
        return true;
}

void AriaCoordinator::setFinishedWorkerState(WorkerState workerState) {
    for(auto worker : workers){
        if(worker.second->workerState == WorkerState::FINISH_COMMIT)
        {
            worker.second->workerState = workerState;
        }
    }
}

