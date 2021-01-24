//
// Created by lhx on 1/21/21.
//

#ifndef MYBLOCKCHAIN_ARIA_COORDINATOR_H
#define MYBLOCKCHAIN_ARIA_COORDINATOR_H

#include <block_server/message_format/aria_global_state.h>
#include <block_server/message_format/worker_state.h>
#include "block_server/coordinator/coordinator.h"
;
class WorkerInstance;
class Config;
class AriaCoordinator : public Coordinator{
public:
    explicit AriaCoordinator(Config &config):Coordinator(config){};

    ~AriaCoordinator() = default;
    void createWorker(int32_t workerID);
    void run() override ;
    void broadToWorkers(AriaGlobalState state);
    void sycnAllWorkers(WorkerState workerState);
    void setWorkerState(int32_t workerID,WorkerState workerState);
    WorkerState getWorkerState(int32_t workerID);



private:
    AriaGlobalState ariaGlobalState;
};
#endif //MYBLOCKCHAIN_ARIA_COORDINATOR_H
