//
// Created by lhx on 1/18/22.
//

#ifndef MYBLOCKCHAIN_TRANSACTIONWRAPPER_H
#define MYBLOCKCHAIN_TRANSACTIONWRAPPER_H
#include <vector>
#include <atomic>
#include "transaction.h"
class TransactionWrapper {
public:
    explicit TransactionWrapper(uint32_t epoch = 0) : _epoch(epoch) {}
    ~TransactionWrapper() = default;
    uint32_t getEpochNumber() const {
        return _epoch;
    }
    void setEpoch(uint32_t epoch) {
        _epoch = epoch;
    }
    uint32_t size() const {
        return transactionList.size();
    }

    bool pushTransaction(Transaction* t) {
        if (t == nullptr)
            return false;
        if (t->getEpoch() != _epoch)
            return false;
        transactionList.push_back(t);
        return true;
    }

    void pushTransactionUncheck(Transaction* t) {
        if(t!= nullptr)
            transactionList.push_back(t);
    }

    void pushTransactionUncheck(std::vector<Transaction*> t) {
        transactionList.insert(transactionList.end(),t.begin(),t.end());
    }

    void pushTransactionUncheck(TransactionWrapper& another) {
        pushTransactionUncheck(another.getTransactionList());
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
    uint32_t _epoch;
    std::vector<Transaction*> transactionList;
};
#endif //MYBLOCKCHAIN_TRANSACTIONWRAPPER_H
