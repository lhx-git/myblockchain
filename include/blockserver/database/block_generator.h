
//
// Created by lhx on 2/11/22.
//

#ifndef MYBLOCKCHAIN_BLOCK_GENERATOR_H
#define MYBLOCKCHAIN_BLOCK_GENERATOR_H
#include <cstdint>
class Transaction;

class BlockGenerator {
public:
    virtual int64_t getLatestSavedEpoch() = 0;
    virtual uint32_t addCommittedTransaction(Transaction* transaction) = 0;
    virtual uint32_t generateBlock() = 0;
};
#endif //MYBLOCKCHAIN_BLOCK_GENERATOR_H
