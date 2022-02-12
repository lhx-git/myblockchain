//
// Created by lhx on 1/18/22.
//

#ifndef MYBLOCKCHAIN_TRANSACTION_EXECUTOR_IMPL_H
#define MYBLOCKCHAIN_TRANSACTION_EXECUTOR_IMPL_H
#include "../transaction_executor.h"
class ReserveTable;
class TransactionExecutorImpl : public TransactionExecutor {
public:
    void executeList(std::vector<Transaction *> transactionList) override;
    void commitList(std::vector<Transaction *> transactionList) override;

private:
    ReserveTable* reserveTable{};
};
#endif //MYBLOCKCHAIN_TRANSACTION_EXECUTOR_IMPL_H
