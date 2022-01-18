//
// Created by lhx on 1/17/22.
//

#ifndef MYBLOCKCHAIN_MOCK_TRANSACTION_H
#define MYBLOCKCHAIN_MOCK_TRANSACTION_H

#include "blockserver/transaction/transaction.h"
class MockTransaction : public Transaction {
public:
    explicit MockTransaction() = default;;
    ~MockTransaction() override = default;
    uint32_t getEpoch() override {
        return _epoch;
    }
    void setEpoch(uint32_t epoch) override {
        _epoch = epoch;
    }
    uint32_t getTransactionID() override {
        return _tid;
    }
    void setTransactionID(uint32_t tid) override {
        _tid = tid;
    }

private:
    uint32_t _epoch{};
    uint32_t _tid{};

};

#endif //MYBLOCKCHAIN_MOCK_TRANSACTION_H
