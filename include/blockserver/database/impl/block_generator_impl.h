//
// Created by lhx on 2/11/22.
//

#ifndef MYBLOCKCHAIN_BLOCK_GENERATOR_IMPL_H
#define MYBLOCKCHAIN_BLOCK_GENERATOR_IMPL_H
#include "blockserver/database/block_generator.h"
#include "common/concurrent_queue/blocking_concurrent_queue.hpp"

namespace Block {
    class Block;
}

class BlockStorage;
class CryptoSign;
class ZMQClient;

class BlockGeneratorImpl: public BlockGenerator {
public:
    BlockGeneratorImpl();
    ~BlockGeneratorImpl();
    int64_t getLatestSavedEpoch() override;
    uint32_t addCommittedTransaction(Transaction* transaction) override;
    uint32_t generateBlock() override;

protected:
    void blockVerifier();
    void getSignatureFromOtherServer(ZMQClient* client);

private:
    moodycamel::BlockingConcurrentQueue<Block::Block*> verifyBuffer;
    std::thread* blockVerifierThread;
    std::vector<std::thread*> remoteReceiverThreads;
    BlockStorage* storage;
    std::atomic<bool> finishSignal;
};

#endif //MYBLOCKCHAIN_BLOCK_GENERATOR_IMPL_H
