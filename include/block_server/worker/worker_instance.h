//
// Created by lhx on 1/21/21.
//

#ifndef MYBLOCKCHAIN_WORKER_INSTANCE_H
#define MYBLOCKCHAIN_WORKER_INSTANCE_H

#include <memory>
#include <thread>
#include <block_server/message_format/worker_state.h>
#include <block_server/message_format/aria_global_state.h>

#endif //MYBLOCKCHAIN_WORKER_INSTANCE_H

class WorkerInstance{
public:
    AriaGlobalState ariaGlobalState;
    WorkerState workerState;
    int32_t workerID;
    std::shared_ptr<Worker> instance;
    std::shared_ptr<std::thread> thread;
};