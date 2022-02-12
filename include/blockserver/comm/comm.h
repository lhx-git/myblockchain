//
// Created by lhx on 2/9/22.
//

#ifndef MYBLOCKCHAIN_COMM_H
#define MYBLOCKCHAIN_COMM_H

#include <string>
class TransactionWrapper;

class Comm {
public:
    virtual TransactionWrapper* getTransaction(uint32_t epoch, uint32_t maxSize, uint32_t minSize) = 0;
    virtual void clearTransactionForEpoch(uint32_t epoch) = 0;
};

#endif //MYBLOCKCHAIN_COMM_H
