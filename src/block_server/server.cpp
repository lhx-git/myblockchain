#include <iostream>
#include "block_server/worker/aria_worker.h"
#include "block_server/coordinator/aria_coordinator.h"
#include "block_server/message_format/config.h"
#include "block_server/transaction/mock/mock_transaction_manager.h"
#include <thread>

int main() {
    Config config;
    config.worker_num = 8;
    std::atomic<int64_t> epochChan;
    auto ariaCoordinator = std::make_shared<AriaCoordinator>(config,epochChan);
    auto transactionManager = std::make_shared<MockTransactionManager>();
    transactionManager->sendTransaction = [ariaCoordinator](std::shared_ptr<TransactionWrapper> trWrapper){
        ariaCoordinator->addTransaction(trWrapper);
    };
    std::thread t1(&AriaCoordinator::run,ariaCoordinator.get());
    std::thread t2(&MockTransactionManager::run,transactionManager.get());
    t2.join();
    t1.join();
}


