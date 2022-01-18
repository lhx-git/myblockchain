//
// Created by lhx on 1/14/22.
//

#ifndef MYBLOCKCHAIN_TRANSACTION_H
#define MYBLOCKCHAIN_TRANSACTION_H
#include <memory>

class Transaction {
public:
    virtual ~Transaction() = default;
    // notice: tid start from 1, not equal 0
    virtual uint32_t getTransactionID() = 0;
    virtual void setTransactionID(uint32_t tid) = 0;

    virtual void setEpoch(uint32_t epoch) = 0;
    virtual uint32_t getEpoch() = 0;
};
#endif //MYBLOCKCHAIN_TRANSACTION_H
