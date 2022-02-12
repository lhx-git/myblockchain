//
// Created by peng on 2/11/22.
//

#ifndef MYBLOCKCHAIN_BLOCK_STORAGE_TO_FILE_H
#define MYBLOCKCHAIN_BLOCK_STORAGE_TO_FILE_H
#include "blockserver/database/block_storage.h"

#include <functional>
#include <queue>
#include <map>
#include <mutex>

class SHA256Helper;
class CryptoSign;

class BlockStorageToFile: public BlockStorage {
public:
    BlockStorageToFile();
    ~BlockStorageToFile() override;

    bool appendBlock(const Block::Block& block) override;
    bool insertBlock(const Block::Block& block) override;
    bool loadBlock(Block::Block& block) override;
    int64_t getLatestSavedEpoch() override;
    uint32_t addPendingTransaction(Transaction* transaction) override;

    // append trs to block, generate tr metadata and add it to block, clear tr queue
    bool generateBlockBody(Block::Block* block) override;
    bool generateBlockHeader(Block::Block* block) override;

protected:
    void initFunc();

    bool updateLatestSavedEpoch(int64_t epoch);
    bool deserializeBlock(uint32_t epoch, Block::Block *block, bool checksum = false);

private:
    CryptoSign* signHelper;
    SHA256Helper* hashHelper;
    std::priority_queue<Transaction*, std::vector<Transaction*>,
            std::function<bool(Transaction*, Transaction*)>> transactionQueue;
    std::mutex saveFileMutex;
};

#endif //MYBLOCKCHAIN_BLOCK_STORAGE_TO_FILE_H
