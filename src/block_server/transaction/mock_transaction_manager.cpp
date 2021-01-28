//
// Created by lhx on 1/26/21.
//
#include "block_server/transaction/mock/mock_transaction_manager.h"
#include "block_server/transaction/transaction.h"
#include "block_server/transaction/mock/mock_transaction.h"
#include "block_server/transaction/transaction_wrapper.h"

#include <memory>

[[noreturn]] void MockTransactionManager::run() {
    uint32_t count =0;
    uint32_t epoch = 0;
    while (true){
        for(int i=0;i<10;i++){
            auto transactionWrapper = std::make_shared<TransactionWrapper>();
            transactionWrapper->setEpoch(epoch);
            Transaction* transaction1 = new MockTransaction;
            transaction1->setEpoch(epoch);
            transaction1->setTransactionID(count++);
            transactionWrapper->pushTransaction(transaction1);
            Transaction* transaction2 = new MockTransaction;
            transaction2->setEpoch(epoch);
            transaction2->setTransactionID(count++);
            transactionWrapper->pushTransaction(transaction2);
            sendTransaction(transactionWrapper);
        }
        epoch++;
    }
    // produce 10 transaction wrapper



}

