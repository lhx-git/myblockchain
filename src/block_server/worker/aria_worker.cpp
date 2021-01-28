//
// Created by lhx on 1/21/21.
//

#include "block_server/worker/aria_worker.h"
#include "iostream"
#include "glog/logging.h"

[[noreturn]] void AriaWorker::run() {
    while (true){
        std::unique_lock<std::mutex> consumeLock(self->mutex);
        while (self->ariaGlobalState!=AriaGlobalState::Aria_READ)
            self->consume.wait(consumeLock);
        self->workerState = WorkerState::FINISH_READ;
        LOG(INFO)<<self->workerID << "finish Aria Read";
        self->produce.notify_one();

        while (self->ariaGlobalState!=AriaGlobalState::Aria_COMMIT)
            self->consume.wait(consumeLock);
        self->workerState = WorkerState::FINISH_COMMIT;
        LOG(INFO)<<self->workerID << "finish Aria commit";
        self->produce.notify_one();
    }

}
