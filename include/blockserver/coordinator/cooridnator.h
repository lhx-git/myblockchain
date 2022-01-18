//
// Created by lhx on 1/13/22.
//
#include <atomic>

#ifndef MYBLOCKCHAIN_COORIDNATOR_H
#define MYBLOCKCHAIN_COORIDNATOR_H
#include  <functional>
class Transaction;

class Coordinator {
public:
    Coordinator(std::atomic<uint32_t>& epochChan) : epochChan(epochChan) {}
    virtual ~Coordinator() {}
    virtual void run() = 0;

    //sync call to add workload, async process it.
    //virtual uint32_t addTransaction(TransactionWrapper* trWrapper) = 0;
    virtual uint32_t addTransaction(Transaction* transaction) = 0;
    std::function<void(uint32_t)> epochTransactionFinishSignal;  //async callback to trManager

protected:
    std::atomic<uint32_t>& epochChan;//用于worker与Coordinator的通信，不是Coordinator与Transaction Manager的
};
#endif //MYBLOCKCHAIN_COORIDNATOR_H
