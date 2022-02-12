//
// Created by lhx on 1/14/22.
//

#ifndef MYBLOCKCHAIN_TRANSACTION_H
#define MYBLOCKCHAIN_TRANSACTION_H
#include <memory>
class KVRWSet;
enum class TransactionResult;
class TransactionPayload;
class Transaction {
public:
    virtual ~Transaction() = default;
    // notice: tid start from 1, not equal 0
    virtual uint32_t getTransactionID() = 0;
    virtual void setTransactionID(uint32_t tid) = 0;

    virtual void setEpoch(uint32_t epoch) = 0;
    virtual uint32_t getEpoch() = 0;

    virtual void resetRWSet() = 0;
    virtual KVRWSet* getRWSet() = 0;

    virtual TransactionResult getTransactionResult() = 0;
    virtual void setTransactionResult(TransactionResult transactionResult) = 0;

    virtual void resetTransaction() = 0;    // if a transaction abort, it must be reset.
    virtual bool isReadOnly() = 0;

    virtual TransactionPayload* getTransactionPayload() = 0;
    virtual void resetTransactionPayload() = 0;

    virtual bool serializeToString(std::string* transactionRaw) = 0;
    virtual bool deserializeFromString(const std::string& transactionRaw) = 0;
};
#endif //MYBLOCKCHAIN_TRANSACTION_H
