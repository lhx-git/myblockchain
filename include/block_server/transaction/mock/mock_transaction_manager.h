//
// Created by lhx on 1/26/21.
//

#ifndef MYBLOCKCHAIN_MOCK_TRANSACTION_MANAGER_H
#define MYBLOCKCHAIN_MOCK_TRANSACTION_MANAGER_H
#include "block_server/transaction/transaction_manager.h"
class TransactionWrapper;
class  MockTransactionManager: public TransactionManager{
public:
    [[noreturn]] void run() ;
};
#endif //MYBLOCKCHAIN_MOCK_TRANSACTION_MANAGER_H
