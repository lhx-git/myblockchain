
//
// Created by lhx on 2/11/22.
//

#ifndef MYBLOCKCHAIN_BLOCK_STORAGE_H
#define MYBLOCKCHAIN_BLOCK_STORAGE_H
#include <string>

namespace Block {
    class Block;
}

class Transaction;

class BlockStorage {
public:
    // return a instance of BlockStorage, init not thread safe
    static BlockStorage* getInstance();
    virtual ~BlockStorage() = default;
    // append a block to the block chain
    virtual bool appendBlock(const Block::Block& block) = 0;
    // also append a block to the block chain
    virtual bool insertBlock(const Block::Block& block) = 0;
    // spec epoch number, fill in rest of the block, thread safe
    virtual bool loadBlock(Block::Block& block) = 0;
    // return max number of block chain, not thread safe!
    virtual int64_t getLatestSavedEpoch() = 0;
    // add a tr to the queue, not thread safe
    virtual uint32_t addPendingTransaction(Transaction* transaction) = 0;
    // generate part of the block, not thread safe
    virtual bool generateBlockBody(Block::Block* block) = 0;
    virtual bool generateBlockHeader(Block::Block* block) = 0;
};
#endif //MYBLOCKCHAIN_BLOCK_STORAGE_H
