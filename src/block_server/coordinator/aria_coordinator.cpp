//
// Created by lhx on 1/21/21.
//

#include "block_server/coordinator/aria_coordinator.h"
#include "block_server/message_format/config.h"
#include "block_server/worker/aria_worker.h"

void AriaCoordinator::run() {
    while (workerID < config.worker_num){
        createWorker(workerID);
        workerID++;
    }
    ariaGlobalState = AriaGlobalState::Aria_READ;
    sycnToWorkers(ariaGlobalState);
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

void AriaCoordinator::sycnToWorkers(AriaGlobalState state) {
    for(auto worker : workers)
    {
        worker.second->ariaGlobalState = this->ariaGlobalState;
    }
}


void AriaCoordinator::setWorkerState(int32_t workerID, WorkerState workerState) {
    workers[workerID]->workerState = workerState;
}

WorkerState AriaCoordinator::getWorkerState(int32_t workerID) {
    return workers[workerID]->workerState;
}




