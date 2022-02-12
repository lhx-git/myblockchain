//
// Created by lhx on 1/19/21.
//

#include "blockserver/transaction/mock/mock_transaction.h"
#include "blockserver/coordinator/AriaCoordinator.h"
#include "blockserver/transaction/mock/mock_transaction_manager.h"
#include "thread"
#include "glog/logging.h"
int main() {
    std::atomic<uint32_t> ep;
    ep.store(0);

    auto coordinator = new AriaCoordinator(ep);
    std::thread coordinatorThread(&AriaCoordinator::run, coordinator);
    auto transactionManager = new MockTransactionManager();
    coordinator->epochTransactionFinishSignal = [&] (uint32_t epoch) {
        transactionManager->lastEpochFinishSignal = epoch;
    };
    transactionManager->sendTransactionHandle = [&] (TransactionWrapper* transactionWrapper) {
        coordinator->addTransaction(transactionWrapper);
    };
    std::thread transactionManagerThread(&TransactionManager::run, transactionManager);
    coordinatorThread.join();
    transactionManagerThread.join();

    return 0;
}