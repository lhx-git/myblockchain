//
// Created by peng on 2021/1/17.
//

#ifndef NEUBLOCKCHAIN_TRANSACTION_H
#define NEUBLOCKCHAIN_TRANSACTION_H

#include <memory>
#include <vector>

class KVRWSet;
class TransactionAction;
enum class TransactionResult;

class Transaction {
public:
    virtual uint32_t getTransactionID() = 0;
    virtual void setTransactionID(uint32_t tid) = 0;
    // search op, and execute op must be done with worker or db!
    // transaction object can not hold a db reference!

    virtual void setRWSet(std::shared_ptr<KVRWSet> kvRWSet) = 0;
    virtual std::shared_ptr<KVRWSet> getRWSet() = 0;

    virtual void resetTransaction() = 0;    // if a transaction abort, it must be reset.
    virtual bool isReadOnly() = 0;

    virtual void setEpoch(uint32_t epoch) = 0;
    virtual uint32_t getEpoch() = 0;

    virtual TransactionResult getTransactionResult() = 0;
    virtual void setTransactionResult(TransactionResult transactionResult) = 0;

    virtual std::shared_ptr<TransactionAction> getTransactionPayload() = 0;
    virtual void setTransactionPayload(std::shared_ptr<TransactionAction> payload) = 0;

};


#endif //NEUBLOCKCHAIN_TRANSACTION_H
