//
// Created by lhx on 1/13/22.
//

#ifndef MYBLOCKCHAIN_ARIACOORDINATOR_H
#define MYBLOCKCHAIN_ARIACOORDINATOR_H
#include <atomic>
#include <map>
#include <queue>
#include <mutex>
#include <condition_variable>
#include "cooridnator.h"
class WorkerInstance;
enum class AriaGlobalState;
enum class WorkerState;
class Transaction;
class Workload;
class AriaCoordinator : public Coordinator {
public:
    explicit AriaCoordinator(std::atomic<uint32_t>& epochChan);

    ~AriaCoordinator() override;
    void run() override;
    uint32_t addTransaction(Transaction* transaction) override;
    //uint32_t addTransaction(TransactionWrapper* trWrapper) override;

protected:
    void mainRoutine();
    //worker management related:
    uint32_t deployWorkload(std::shared_ptr<Workload> workload);

    //woker management related
    uint32_t createWorker(std::shared_ptr<Workload> workload);
    uint32_t destroyUnusedWorkers();
    void printWorkerCondition();

    //信号相关
    void broadcastToAllWorker(bool onlyAssigned = true);

    //worker同步相关
    void syncAllWorker(WorkerState state, bool onlyAssigned = true);
    void setWorkerState(int workerID, AriaGlobalState state);
    void syncWorkerState(uint32_t workerID, WorkerState state);

    // others:
    void getWorkload(uint32_t& isCurrentEpochEmpty, std::shared_ptr<Workload>& workload);



private:
    std::map<uint32_t, WorkerInstance*> workers; //workerID, WorkerInstance
    AriaGlobalState globalState;
    uint32_t IDNounce; // 最大的Worker的 workerID + 1

    std::mutex transactionBufferMutex;
    std::queue<Transaction*> transactionBuffer;
    std::condition_variable produce, consume;
};
#endif //MYBLOCKCHAIN_ARIACOORDINATOR_H
