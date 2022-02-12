//
// Created by lhx on 1/18/22.
//

#ifndef MYBLOCKCHAIN_TRANSACTION_MANAGER_H
#define MYBLOCKCHAIN_TRANSACTION_MANAGER_H
#include <functional>
#include <atomic>
class TransactionWrapper;
class TransactionManager {
public:
    TransactionManager() : lastEpochFinishSignal(0) {}
    virtual ~TransactionManager() = default;;
    virtual void run() = 0;//开启一个transactionManager
    //调用coordinator的addTransaction方法
    std::function<void(TransactionWrapper*)> sendTransactionHandle;
    //已经写入数据库的最新的epoch
    std::atomic<int32_t> lastEpochFinishSignal;
};
#endif //MYBLOCKCHAIN_TRANSACTION_MANAGER_H
