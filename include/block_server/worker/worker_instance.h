//
// Created by lhx on 1/21/21.
//

#ifndef MYBLOCKCHAIN_WORKER_INSTANCE_H
#define MYBLOCKCHAIN_WORKER_INSTANCE_H

#include <memory>
#include <thread>
#include <block_server/message_format/worker_state.h>
#include <block_server/message_format/aria_global_state.h>
#include <mutex>
#include <condition_variable>
#include "block_server/message_format/workload.h"

#endif //MYBLOCKCHAIN_WORKER_INSTANCE_H

class WorkerInstance{
public:
    AriaGlobalState ariaGlobalState;
    WorkerState workerState;
    int32_t workerID;
    std::shared_ptr<Worker> instance;
    std::shared_ptr<std::thread> thread;
    std::mutex mutex;
    std::condition_variable produce , consume;
    std::shared_ptr<Workload> workload;
    std::int64_t epoch;
};