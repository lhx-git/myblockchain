//
// Created by peng on 2/17/21.
//

#include "blockserver/database/impl/block_generator_impl.h"
#include "blockserver/database/block_storage.h"
#include "blockserver/transaction/transaction.h"
#include "glog/logging.h"
#include "common/merkle_tree/merkle_tree.h"
#include "common/yaml_config.h"
#include "common/base64.h"
#include "common/zmq/zmq_server.h"
#include "common/zmq/zmq_client.h"
#include "block.pb.h"
#include "comm.pb.h"

int64_t BlockGeneratorImpl::getLatestSavedEpoch() {
    return storage->getLatestSavedEpoch();
}

uint32_t BlockGeneratorImpl::addCommittedTransaction(Transaction *transaction) {
    return storage->addPendingTransaction(transaction);
}

uint32_t BlockGeneratorImpl::generateBlock() {
    auto* block = new Block::Block;
    if(!storage->generateBlockBody(block)) {
        LOG(ERROR) << "generate block body failed!";
        return false;
    }
    if(!storage->generateBlockHeader(block)) {
        LOG(ERROR) << "generate block header failed!";
        return false;
    }
    if(!storage->appendBlock(*block)) {
        LOG(ERROR) << "save current block failed!";
        return false;
    }
    // sign the block and broadcast the block async
    verifyBuffer.enqueue(block);
    return true;
}

#define D_TEST_VALUES 17

BlockGeneratorImpl::BlockGeneratorImpl() :finishSignal(false) {
    // init storage
    storage = BlockStorage::getInstance();
//    blockVerifierThread = new std::thread(&BlockGeneratorImpl::blockVerifier, this);
//    // 1.1. Establish server to server connection
//    for (const auto& ip: YAMLConfig::getInstance()->getBlockServerIPs()) {
//        if(ip == YAMLConfig::getInstance()->getLocalBlockServerIP()) {
//            continue;
//        }
//        // 1.2. bring up remote receiver
//        remoteReceiverThreads.push_back(
//                new std::thread(&BlockGeneratorImpl::getSignatureFromOtherServer,
//                                this,
//                                new ZMQClient(ip, "7002", zmq::socket_type::sub))
//        );
//    }
    //test case
    {
        mt_t *mt = mt_create();
        uint8_t test_values[D_TEST_VALUES][HASH_LENGTH];
        for (uint32_t i = 0; i < D_TEST_VALUES; ++i) {
            for (uint32_t c = 0; c < HASH_LENGTH; ++c) {
                test_values[i][c] = i;
            }
        }
        unsigned char ROOT_5_1[HASH_LENGTH] = {0x24, 0x39, 0x37, 0xFE, 0x91, 0xB8,
                                               0xAF, 0xCC, 0xF7, 0x79, 0x51, 0xAF, 0x4E, 0x94, 0x6C, 0x99, 0x3E, 0x21,
                                               0xCF, 0xE1, 0x34, 0x64, 0x4F, 0xAD, 0x15, 0xDA, 0x30, 0x2E, 0xF0, 0x93,
                                               0xAE, 0x68};
        unsigned char ROOT_5_2[HASH_LENGTH] = {0x8F, 0x6F, 0xA8, 0x61, 0x7E, 0x7B,
                                               0x9D, 0x00, 0x4C, 0xF8, 0xE7, 0x4B, 0x54, 0x25, 0xF9, 0xA5, 0xA7, 0x27,
                                               0x37, 0xFD, 0xE3, 0xAD, 0x35, 0x73, 0xBB, 0xD1, 0x81, 0xD8, 0xD2, 0x60,
                                               0x82, 0x7F};
        mt_hash_t root;
        memset(root, 0, sizeof(mt_hash_t));
        for (uint32_t i = 0; i < 5; ++i) {
            CHECK(mt_add(mt, test_values[i], HASH_LENGTH) == MT_SUCCESS);
        }
        for (uint32_t i = 0; i < 5; ++i) {
            CHECK(
                    mt_verify(mt, test_values[i], HASH_LENGTH, i) == MT_SUCCESS);
        }
        CHECK(mt_get_root(mt, root) == MT_SUCCESS);
        CHECK(!memcmp(ROOT_5_1, root, HASH_LENGTH));
        CHECK(mt_update(mt, test_values[7], HASH_LENGTH, 0) == MT_SUCCESS);
        CHECK(mt_get_root(mt, root) == MT_SUCCESS);
        CHECK(!memcmp(ROOT_5_2, root, HASH_LENGTH));

        mt_delete(mt);
    }
    {
        mt_t *mt = mt_create();
        uint8_t test_values[D_TEST_VALUES][HASH_LENGTH];
        for (uint32_t i = 0; i < D_TEST_VALUES; ++i) {
            for (uint32_t c = 0; c < HASH_LENGTH; ++c) {
                test_values[i][c] = i;
            }
        }

        for (uint32_t i = 0; i < 3; ++i) {
            CHECK(mt_add(mt, test_values[i], sizeof(mt_hash_t)) == MT_SUCCESS);
        }
        CHECK(mt_update(mt, test_values[3], sizeof(mt_hash_t), 2) == MT_SUCCESS);

        mt_delete(mt);
    }
}

BlockGeneratorImpl::~BlockGeneratorImpl() {
    finishSignal = true;
    blockVerifierThread->join();
    delete blockVerifierThread;
    delete storage;
}

void BlockGeneratorImpl::blockVerifier() {
    Block::Block* block;
    ZMQServer server("0.0.0.0","7002", zmq::socket_type::pub);
    while (!finishSignal) {
        verifyBuffer.wait_dequeue(block);
        CHECK(block->metadata().metadata_size() >= 3);
        // 1. trMetadata
        // 2. merkleRoot
        // 3. block signature
        auto* signatureExchange = new comm::SignatureExchange;
        signatureExchange->set_signature(block->metadata().metadata(2));
        signatureExchange->set_epoch(block->header().number());
        delete block;
        // broadcast the signature
        server.sendReply(signatureExchange->SerializeAsString());
    }
}

void BlockGeneratorImpl::getSignatureFromOtherServer(ZMQClient *client) {
    // code re-enter, connect to other server
    while(!finishSignal) {
        std::string messageRaw;
        client->getReply(messageRaw);
        auto* signatureExchange = new comm::SignatureExchange;
        signatureExchange->ParseFromString(messageRaw);
        DLOG(INFO) << "Exchange epoch: " << signatureExchange->epoch() <<", signature: " << base64_encode(signatureExchange->signature());
    }
    delete client;
}
