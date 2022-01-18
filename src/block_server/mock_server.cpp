//
// Created by lhx on 1/19/21.
//

#include "blockserver/transaction/mock/mock_transaction.h"
#include "blockserver/coordinator/AriaCoordinator.h"
#include "thread"
#include "glog/logging.h"
int main() {
    int epoch = 1;
    int tid = 1;
    std::atomic<uint32_t> ep;
    ep.store(1);
    auto coordinator = new AriaCoordinator(ep);
    std::thread t1(&AriaCoordinator::run, coordinator);
    coordinator->epochTransactionFinishSignal = [&] (uint32_t ) {
        epoch++;
    };
    for (int i = 0; i < 10; i++) {
        Transaction *transaction = new MockTransaction();
        transaction->setTransactionID(tid);
        transaction->setEpoch(epoch);
        tid++;
        coordinator->addTransaction(transaction);
    }
    Transaction *transaction1 = new MockTransaction();
    transaction1->setTransactionID(tid);
    transaction1->setEpoch(epoch + 1);
    tid++;
    coordinator->addTransaction(transaction1);

    LOG(INFO) << "finish epoch 1" << std::endl;
    while (epoch < 2) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    LOG(INFO) << "start epoch 2" << std::endl;
    for (int i = 0; i < 11; i++) {
        Transaction *transaction = new MockTransaction();
        transaction->setTransactionID(tid);
        transaction->setEpoch(epoch);
        tid++;
        coordinator->addTransaction(transaction);
    }
    Transaction *transaction2 = new MockTransaction();
    transaction2->setTransactionID(tid);
    transaction2->setEpoch(epoch + 1);
    tid++;
    coordinator->addTransaction(transaction2);
    t1.join();
    return 0;
}