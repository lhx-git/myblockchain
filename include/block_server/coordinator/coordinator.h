//
// Created by lhx on 1/21/21.
//

#ifndef MYBLOCKCHAIN_COORDINATOR_H
#define MYBLOCKCHAIN_COORDINATOR_H
#include "cstdlib"
#include <map>
#include <atomic>
#include <functional>

class TransactionWrapper;
class WorkerInstance;
class Config;
class Coordinator{
public:
    Coordinator(Config &config,std::atomic<int64_t>& epochChan):config(config),epochChan(epochChan),coordinatorID(0){};
    ~Coordinator() = default;
    virtual void run() = 0;
    //sync call to add workload, async process it.
    virtual uint32_t addTransaction(std::shared_ptr<TransactionWrapper> trWrapper) = 0;   //传递shared_ptr参数用传值


    std::function<void(uint32_t)> epochTransactionFinishSignal;  //async callback to trManager


protected:
    int32_t coordinatorID;
    Config &config;
    std::atomic<int64_t>& epochChan;
};
#endif //MYBLOCKCHAIN_COORDINATOR_H
