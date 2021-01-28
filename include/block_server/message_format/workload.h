//
// Created by lhx on 1/28/21.
//

#ifndef MYBLOCKCHAIN_WORKLOAD_H
#define MYBLOCKCHAIN_WORKLOAD_H
#include "cstdlib"
#include <vector>
class Transaction;
class Workload{
public:
    int64_t getEpochNumber() const{return this->epoch; }
    void setEpochNumber(int64_t epoch){ this->epoch = epoch; }
    bool pushTransaction(Transaction* t){
        if(t!= nullptr)
        {
            this->transactionList.push_back(t);
        }
    }
    Transaction* popTransaction(){
        Transaction* t  = transactionList.back();
        transactionList.pop_back();
        return t;

    }
    bool pushTransaction(std::vector<Transaction*> t){
        if(!t.empty())
        {
            transactionList.insert(transactionList.end(),t.begin(),t.end());
        }
    }
    std::vector<Transaction*> getTransactionList(){return this->transactionList; }
private:
    int64_t epoch;
    std::vector<Transaction*> transactionList;

};
#endif //MYBLOCKCHAIN_WORKLOAD_H
