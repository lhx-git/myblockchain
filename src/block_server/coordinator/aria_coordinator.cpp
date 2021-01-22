//
// Created by lhx on 1/21/21.
//

#include "block_server/coordinator/aria_coordinator.h"
#include "block_server/message_format/config.h"
#include "block_server/worker/aria_worker.h"
#include "block_server/worker/worker_instance.h"

void AriaCoordinator::run() {
    while (workerID < config.worker_num){
        createWorker(workerID);
        workerID++;
    }

    /*for(auto worker :workers)
    {
        worker.second->thread->join();
    }*/
}

void AriaCoordinator::createWorker(int32_t workerID) {
    auto worker = new WorkerInstance;
    workers[workerID] = worker;
    worker->instance = std::make_shared<AriaWorker>(workerID);  // ploymorphism
    worker->thread = std::make_shared<std::thread>(&Worker::run,worker->instance.get());
}




