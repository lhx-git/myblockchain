//
// Created by lhx on 2/11/22.
//

#include "blockserver/database/impl/block_storage_to_file.h"
#include <fstream>
#include "common/yaml_config.h"
#include "common/crypto_sign.h"
#include "common/base64.h"
#include "common/merkle_tree/merkle_tree.h"
#include "glog/logging.h"

#include "blockserver/transaction/transaction.h"
#include "common/sha256_hepler.h"
#include "block.pb.h"

BlockStorageToFile::BlockStorageToFile()
        :transactionQueue([](Transaction* a, Transaction* b) {
    return a->getTransactionID()  < b->getTransactionID(); }) {
    hashHelper = new SHA256Helper;
    auto* config = YAMLConfig::getInstance();
    auto blockServerConfig = config->getBlockServerConfig(config->getLocalBlockServerIP());
    signHelper = new CryptoSign(blockServerConfig->usrPubFilePath, blockServerConfig->priFilePath);
    if(config->resetEpoch()) {
        initFunc();
    }
}

BlockStorageToFile::~BlockStorageToFile() {
    delete hashHelper;
    delete signHelper;
}

void BlockStorageToFile::initFunc() {
    // reset block storage
    std::fstream ofs2("block_num.txt", std::ios::binary|std::ios::out);
    if(!ofs2)
        return;
    ofs2 << 0;
    ofs2.close();
}

bool BlockStorageToFile::insertBlock(const Block::Block& block) {
    std::unique_lock<std::mutex> lock(saveFileMutex);
    try{
        int64_t epoch = block.header().number();
        std::fstream ofs(std::to_string(epoch) + ".bin", std::ios::binary|std::ios::out);
        if(!ofs || !block.SerializeToOstream(&ofs))
            return false;
        ofs.close();
    } catch (const std::exception& e) {
        LOG(ERROR) << e.what();
        return false;
    }
    return true;
}

bool BlockStorageToFile::loadBlock(Block::Block& block) {
    std::unique_lock<std::mutex> lock(saveFileMutex);
    try{
        int64_t epoch = block.header().number();
        std::fstream ifs(std::to_string(epoch) + ".bin", std::ios::binary|std::ios::in);
        if(!ifs || !block.ParseFromIstream(&ifs))
            return false;
        ifs.close();
    } catch (const std::exception& e) {
        LOG(ERROR) << e.what();
        return false;
    }
    return true;
}

int64_t BlockStorageToFile::getLatestSavedEpoch() {
    int64_t epoch;
    std::unique_lock<std::mutex> lock(saveFileMutex);
    try{
        std::fstream ifs("block_num.txt", std::ios::binary|std::ios::in);
        if(!ifs)
            return false;
        ifs >> epoch;
        ifs.close();
    } catch (const std::exception& e) {
        LOG(ERROR) << e.what();
        return 0;
    }
    return epoch;
}

bool BlockStorageToFile::deserializeBlock(uint32_t epoch, Block::Block *block, bool checksum) {
    block->mutable_header()->set_number(epoch);
    if(!loadBlock(*block)) {
        LOG(WARNING) << "read block error!";
        return false;
    }
    std::string hashResult;
    if(!(hashHelper->hash(block->data().SerializeAsString())&&
         hashHelper->append(block->metadata().SerializeAsString())&&
         hashHelper->execute(&hashResult))) {
        return false;
    }
    if(hashResult != block->header().data_hash()) {
        return false;
    }
    // check if the signature is correct
    if(checksum) {
        std::string digest;
        mt_t *mt = mt_create();
        for(auto& data: block->data().data()) {
            hashHelper->hash(data, &digest);
            CHECK(mt_add(mt, reinterpret_cast<const uint8_t *>(digest.data()), HASH_LENGTH) == MT_SUCCESS);
        }
        std::string root;
        root.resize(HASH_LENGTH);
        CHECK(mt_get_root(mt, reinterpret_cast<uint8_t *>(root.data())) == MT_SUCCESS);
        mt_delete(mt);
        if(block->metadata().metadata_size() < 2) {
            return false;
        }
        auto& blkSignature = block->metadata().metadata(0);
        if(!signHelper->rsaDecrypt(root, blkSignature)){
            return false;
        }
    }
    return true;
}

uint32_t BlockStorageToFile::addPendingTransaction(Transaction *transaction) {
    transactionQueue.push(transaction);
    return true;
}

bool BlockStorageToFile::generateBlockBody(Block::Block *block) {
    // add metadata of transaction
    auto* trMetadata = block->mutable_metadata()->add_metadata();
    // add metadata of merkle root (not necessary)
    auto* merkleRoot = block->mutable_metadata()->add_metadata();
    // add metadata of block second
    auto* blkSignature = block->mutable_metadata()->add_metadata();
    Block::BlockData* blockData = block->mutable_data();
    while (!transactionQueue.empty()) {
        Transaction* transaction = transactionQueue.top();
        std::string* data = blockData->add_data();
        CHECK(transaction->serializeToString(data));
        // write metadata of transaction first
        trMetadata->push_back((char)transaction->getTransactionResult());
        transactionQueue.pop();
    }
    // write metadata of block
    // metadata[0]: block signature
    // metadata[1]: transaction result(abort or success)
    std::string digest;
    mt_t *mt = mt_create();
    for(auto& data: blockData->data()) {
        hashHelper->hash(data, &digest);
        CHECK(mt_add(mt, reinterpret_cast<const uint8_t *>(digest.data()), HASH_LENGTH) == MT_SUCCESS);
    }
    std::string root;
    root.resize(HASH_LENGTH);
    CHECK(mt_get_root(mt, reinterpret_cast<uint8_t *>(root.data())) == MT_SUCCESS);
    mt_delete(mt);
    CHECK(signHelper->rsaEncrypt(root, blkSignature));
    DLOG(INFO) << "block signature: " << base64_encode(*blkSignature);
    return true;
}

bool BlockStorageToFile::generateBlockHeader(Block::Block *block) {
    // generate header
    std::string serializedBLockMetadata = block->metadata().SerializeAsString();
    std::string serializedBLockData = block->data().SerializeAsString();

    std::string hashResult;
    if(!(hashHelper->hash(block->data().SerializeAsString())&&
         hashHelper->append(block->metadata().SerializeAsString())&&
         hashHelper->execute(&hashResult))){
        return false;
    }
    Block::BlockHeader* header = block->mutable_header();
    // sst hash, TODO: optimize
    Block::Block previous;
    int64_t last_epoch = getLatestSavedEpoch();
    header->set_data_hash(hashResult);
    header->set_number(last_epoch + 1);
    if(last_epoch > 0) {
        CHECK(deserializeBlock(last_epoch, &previous));
        header->set_previous_hash(previous.header().data_hash());
    }
    return true;
}

bool BlockStorageToFile::appendBlock(const Block::Block &block) {
    if(!insertBlock(block)){
        return false;
    }
    if(!updateLatestSavedEpoch(block.header().number())) {
        return false;
    }
    return true;
}

bool BlockStorageToFile::updateLatestSavedEpoch(int64_t epoch) {
    std::unique_lock<std::mutex> lock(saveFileMutex);
    try{
        std::fstream ofs2("block_num.txt", std::ios::binary|std::ios::out);
        if(!ofs2)
            return false;
        ofs2 << epoch;
        ofs2.close();
    } catch (const std::exception& e) {
        LOG(ERROR) << e.what();
        return false;
    }
    return true;
}
