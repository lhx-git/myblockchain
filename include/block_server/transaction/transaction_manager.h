//
// Created by lhx on 1/25/21.
//

#ifndef MYBLOCKCHAIN_TRANSACTION_MANAGER_H
#define MYBLOCKCHAIN_TRANSACTION_MANAGER_H

#include <memory>
#include <functional>
class TransactionWrapper;
class TransactionManager{
public:
    virtual void run() = 0;
    std::function<void(std::shared_ptr<TransactionWrapper>)> sendTransaction;
    uint32_t lastEpochFinishSignal; //last epoch number
};
#endif //MYBLOCKCHAIN_TRANSACTION_MANAGER_H
