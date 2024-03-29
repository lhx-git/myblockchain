//
// Created by lhx on 1/14/22.
//

#ifndef MYBLOCKCHAIN_ARIAWORKER_H
#define MYBLOCKCHAIN_ARIAWORKER_H

#include "worker.h"
class WorkerInstance;
class TransactionExecutor;

enum class WorkerState;
enum class AriaGlobalState;

class AriaWorker : public Worker {
public:
    explicit AriaWorker(WorkerInstance* self);
    ~AriaWorker() override;
    void run() override;

protected:
    void setWorkerState(WorkerState state);
    bool waitForSignal(AriaGlobalState _state);
private:
    std::shared_ptr<TransactionExecutor> transactionExecutor;
};
#endif //MYBLOCKCHAIN_ARIAWORKER_H
