//
// Created by lhx on 1/21/21.
//

#include "block_server/worker/aria_worker.h"
#include "iostream"
#include "common/hash_map.h"
void AriaWorker::run() {
    std::cout<<"run  "<< this->self->workerID;

}
