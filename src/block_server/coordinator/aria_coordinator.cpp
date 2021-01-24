//
// Created by lhx on 1/21/21.
//

#include "block_server/coordinator/aria_coordinator.h"
#include "block_server/message_format/config.h"
#include "block_server/worker/aria_worker.h"
#include "glog/logging.h"
void AriaCoordinator::run() {
    while (workerID < config.worker_num){
        createWorker(workerID);
        workerID++;
    }
    int count = 0;
    while (count++<3){
        LOG(INFO)<< "start aria read";
        ariaGlobalState = AriaGlobalState::Aria_READ;
        broadToWorkers(ariaGlobalState);
        sycnAllWorkers(WorkerState::FINISH_READ);
        ariaGlobalState = AriaGlobalState::Aria_COMMIT;
        LOG(INFO)<< "start aria commit";
        broadToWorkers(ariaGlobalState);
        sycnAllWorkers(WorkerState::FINISH_COMMIT);
        LOG(INFO)<< "finish aria commit";
    }



}

void AriaCoordinator::createWorker(int32_t workerID) {
    auto worker = new WorkerInstance;
    workers[workerID] = worker;
    worker->ariaGlobalState = AriaGlobalState::START;
    worker->workerState = WorkerState::READY;
    worker->workerID = workerID;
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




