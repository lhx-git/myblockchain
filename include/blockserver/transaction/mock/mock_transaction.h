//
// Created by lhx on 1/17/22.
//

#ifndef MYBLOCKCHAIN_MOCK_TRANSACTION_H
#define MYBLOCKCHAIN_MOCK_TRANSACTION_H

#include "blockserver/transaction/transaction.h"
#include "blockserver/message_fomat/transaction_result.h"
#include "transaction.pb.h"
#include "kv_rwset.pb.h"
class MockTransaction : public Transaction {
public:
    explicit MockTransaction(uint32_t epoch, uint32_t tid);
    explicit MockTransaction();
    ~MockTransaction() override ;

    uint32_t getEpoch() override ;
    void setEpoch(uint32_t epoch) override ;

    uint32_t getTransactionID() override ;
    void setTransactionID(uint32_t tid) override ;

    void resetTransaction() override;    // if a transaction abort, it must be reset.
    bool isReadOnly() override;

    TransactionResult getTransactionResult() override ;
    void setTransactionResult(TransactionResult transactionResult) override ;

    KVRWSet* getRWSet() override ;
    void resetRWSet() override ;

    TransactionPayload* getTransactionPayload() override ;
    void resetTransactionPayload() override ;

    bool serializeToString(std::string *transactionRaw) override;
    bool deserializeFromString(const std::string &transactionRaw) override;



private:
    uint32_t _epoch{};
    uint32_t _tid{};
    TransactionResult _transactionResult;
    TransactionPayload* _transactionPayload;
    KVRWSet* _kvrwSet;

};

#endif //MYBLOCKCHAIN_MOCK_TRANSACTION_H
