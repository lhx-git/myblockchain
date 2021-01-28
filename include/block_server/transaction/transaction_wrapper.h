//
// Created by peng on 2021/1/18.
//

#ifndef NEUBLOCKCHAIN_TRANSACTION_WRAPPER_H
#define NEUBLOCKCHAIN_TRANSACTION_WRAPPER_H

#include <vector>
#include "block_server/transaction/transaction.h"

class TransactionWrapper {
public:
    explicit  TransactionWrapper(uint32_t _epoch=0) :epoch(_epoch) {}

    uint32_t getEpochNumber() const {
        return this->epoch;
    }
    void setEpoch(uint32_t _epoch) {
        this->epoch = _epoch;
    }

    uint32_t size() const {
        if(transactionList.empty())
            return 0;
        return transactionList.size();
    }

    bool pushTransaction(Transaction* t) {
        if(t== nullptr)
            return false;
        if(t->getEpoch()!=epoch)
            return false;
        transactionList.push_back(t);
        return true;
    }

    void pushTransactionUncheck(Transaction* t) {
        transactionList.push_back(t);
    }

    void pushTransactionUncheck(std::vector<Transaction*> t) {
        transactionList.insert(transactionList.end(),t.begin(),t.end());
    }

    void pushTransactionUncheck(TransactionWrapper& another) {
        this->pushTransactionUncheck(another.getTransactionList());
    }

    Transaction* popTransaction() {
        Transaction* tr = transactionList.back();
        transactionList.pop_back();
        return tr;
    }

    std::vector<Transaction*> getTransactionList() {
        return this->transactionList;
    }

private:
    uint32_t epoch;
    std::vector<Transaction*> transactionList;
};


#endif //NEUBLOCKCHAIN_TRANSACTION_WRAPPER_H
