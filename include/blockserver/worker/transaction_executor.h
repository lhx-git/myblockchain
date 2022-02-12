//
// Created by lhx on 1/18/22.
//

#ifndef MYBLOCKCHAIN_TRANSACTION_EXECUTOR_H
#define MYBLOCKCHAIN_TRANSACTION_EXECUTOR_H
#include <vector>
class Transaction;
class TransactionExecutor {
public:
    virtual void executeList(std::vector<Transaction*> transactionList) = 0;
    virtual void commitList(std::vector<Transaction*> transactionList) = 0;
};
#endif //MYBLOCKCHAIN_TRANSACTION_EXECUTOR_H
