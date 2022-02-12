//
// Created by lhx on 1/18/22.
//

#ifndef MYBLOCKCHAIN_MOCK_TRANSACTION_MANAGER_H
#define MYBLOCKCHAIN_MOCK_TRANSACTION_MANAGER_H
#include "../transaction_manager.h"

class MockTransactionManager : public TransactionManager {
public:
    void run() override;
    ~MockTransactionManager() override;

private:
    TransactionWrapper* trWrapper;
};
#endif //MYBLOCKCHAIN_MOCK_TRANSACTION_MANAGER_H
