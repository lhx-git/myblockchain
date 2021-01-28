//
// Created by lhx on 1/26/21.
//

#ifndef MYBLOCKCHAIN_MOCK_TRANSACTION_H
#define MYBLOCKCHAIN_MOCK_TRANSACTION_H
#include "cstdlib"
class Transaction;
class MockTransaction: public Transaction{
public:
     uint32_t getTransactionID() {return this->tid;};
     void setTransactionID(uint32_t tid) { this->tid = tid;};
    // search op, and execute op must be done with worker or db!
    // transaction object can not hold a db reference!

     void setRWSet(std::shared_ptr<KVRWSet> kvRWSet) { this->kvRWSet=kvRWSet;}
     std::shared_ptr<KVRWSet> getRWSet() {return this->kvRWSet;}

     void resetTransaction() {}   // if a transaction abort, it must be reset.
     bool isReadOnly() {return  false;}

     void setEpoch(uint32_t epoch) { this->epoch = epoch;}
     uint32_t getEpoch() {return this->epoch;}

    TransactionResult getTransactionResult() { return this->transactionResult;}
     void setTransactionResult(TransactionResult transactionResult) { this->transactionResult = transactionResult;};

    std::shared_ptr<TransactionAction> getTransactionPayload() { return this->payload;};
    virtual void setTransactionPayload(std::shared_ptr<TransactionAction> payload) { this->payload = payload;};

    uint32_t epoch;
    uint64_t tid;
    std::shared_ptr<KVRWSet> kvRWSet;
    TransactionResult transactionResult;
    std::shared_ptr<TransactionAction> payload;

};
#endif //MYBLOCKCHAIN_MOCK_TRANSACTION_H
