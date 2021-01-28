//
// Created by lhx on 1/21/21.
//

#ifndef MYBLOCKCHAIN_ARIA_COORDINATOR_H
#define MYBLOCKCHAIN_ARIA_COORDINATOR_H

#include <block_server/message_format/aria_global_state.h>
#include <block_server/message_format/worker_state.h>
#include <mutex>
#include <queue>
#include <condition_variable>
#include "block_server/coordinator/coordinator.h"
;
class WorkerInstance;
class Config;
class Transaction;
class Workload;
class AriaCoordinator : public Coordinator{
public:
    explicit AriaCoordinator(Config &config, std::atomic<int64_t>& epochChan): Coordinator(config, epochChan){};

    ~AriaCoordinator() = default;
    void createWorker(int32_t workerID);

    [[noreturn]] void run() override ;
    void broadToWorkers(AriaGlobalState state);
    void sycnAllWorkers(WorkerState workerState);
    void setWorkerState(int32_t workerID,WorkerState workerState);
    void setFinishedWorkerState(WorkerState workerState);
    WorkerState getWorkerState(int32_t workerID);

    uint32_t addTransaction(std::shared_ptr<TransactionWrapper> trWrapper) override;  //called by tr_manager
    bool getWorkload(bool& isCurrentEpochEmpty,std::shared_ptr<Workload>& workload);
    bool deployWorker(std::shared_ptr<Workload> workload);


private:
    AriaGlobalState ariaGlobalState;
    std::map<int32_t ,WorkerInstance*> workers; //workerID, workerInstance
    uint32_t workerID;   //the biggest worker instance + 1
    std::mutex transactionBufferMutex;
    std::queue<Transaction*> transactionBuffer;
    std::condition_variable produce, consume;
};
#endif //MYBLOCKCHAIN_ARIA_COORDINATOR_H


