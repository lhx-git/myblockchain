//
// Created by lhx on 1/18/22.
//

#include "blockserver/transaction/mock/mock_transaction.h"
#include "common/sha256_hepler.h"
#include "glog/logging.h"

MockTransaction::MockTransaction(uint32_t epoch, uint32_t tid) : _epoch(epoch), _tid(tid) {
    _kvrwSet = new KVRWSet();
    _transactionResult = TransactionResult::PENDING;
    _transactionPayload = new TransactionPayload;
}

MockTransaction::MockTransaction() : _epoch(0), _tid(0) {
    _kvrwSet = new KVRWSet();
    _transactionResult = TransactionResult::PENDING;
    _transactionPayload = new TransactionPayload;
}

MockTransaction::~MockTransaction() {
    delete _kvrwSet;
    delete _transactionPayload;
}

uint32_t MockTransaction::getEpoch() {
    return _epoch;
}

void MockTransaction::setEpoch(uint32_t epoch) {
    _epoch = epoch;
}

void MockTransaction::setTransactionID(uint32_t tid) {
    _tid = tid;
}

uint32_t MockTransaction::getTransactionID() {
    return _tid;
}

KVRWSet *MockTransaction::getRWSet() {
    return _kvrwSet;
}

void MockTransaction::resetRWSet() {
    _kvrwSet->Clear();
}

TransactionResult MockTransaction::getTransactionResult() {
    return _transactionResult;
}

void MockTransaction::setTransactionResult(TransactionResult transactionResult) {
    _transactionResult = transactionResult;
}

TransactionPayload *MockTransaction::getTransactionPayload() {
    return _transactionPayload;
}

void MockTransaction::resetTransactionPayload() {
    _transactionPayload->Clear();
}

void MockTransaction::resetTransaction() {
    _epoch = 0;
    _kvrwSet->Clear();
    _transactionResult = TransactionResult::PENDING;
}

bool MockTransaction::isReadOnly() {
    return _kvrwSet->writes().empty();
}

bool MockTransaction::serializeToString(std::string *transactionRaw) {
    // transactionRaw is empty
    transactionRaw->clear();
    google::protobuf::io::StringOutputStream stringOutputStream(transactionRaw);
    auto* codedOutputStream = new google::protobuf::io::CodedOutputStream(&stringOutputStream);
    // tid: int64
    codedOutputStream->WriteVarint64(_tid);
    // epoch int64
    codedOutputStream->WriteVarint64(_epoch);
    // payload int64 + string
    std::string payload = _transactionPayload->SerializeAsString();
    codedOutputStream->WriteVarint64(payload.size());
    codedOutputStream->WriteString(payload);
    // rwset int64 + string
    std::string rwSet = _kvrwSet->SerializeAsString();
    codedOutputStream->WriteVarint64(rwSet.size());
    codedOutputStream->WriteString(rwSet);
    // Destroy the CodedOutputStream and position the underlying
    // ZeroCopyOutputStream immediately after the last byte written.
    delete codedOutputStream;

    SHA256Helper hashHelper;
    std::string hashResult;
    if(!hashHelper.hash(*transactionRaw, &hashResult)) {
        return false;
    }
    transactionRaw->append(hashResult);
    return true;
}

bool MockTransaction::deserializeFromString(const std::string &transactionRaw) {
    // data start at tr head
    google::protobuf::io::ArrayInputStream arrayInputStream(transactionRaw.data(), transactionRaw.size());
    google::protobuf::io::CodedInputStream codedInputStream(&arrayInputStream);
    // read uint64
    {
        uint64_t tid;
        if (!codedInputStream.ReadVarint64(&tid)) {
            return false;
        }
        _tid = tid;
    }
    // read uint64
    {
        uint64_t epoch;
        if (!codedInputStream.ReadVarint64(&epoch)) {
            return false;
        }
        _epoch = epoch;
    }
    // read size + payload
    {
        uint64_t payloadSize;
        if (!codedInputStream.ReadVarint64(&payloadSize)) {
            return false;
        }
        std::string payloadString;
        if (!codedInputStream.ReadString(&payloadString, payloadSize)) {
            return false;
        }
        CHECK(_transactionPayload->ParseFromString(payloadString));
    }
    // read size + rwset
    {
        uint64_t rwSetSize;
        if (!codedInputStream.ReadVarint64(&rwSetSize)) {
            return false;
        }
        std::string rwSetString;
        if (!codedInputStream.ReadString(&rwSetString, rwSetSize)) {
            return false;
        }
        CHECK(_kvrwSet->ParseFromString(rwSetString));
    }
    // phase hash
    {
        std::string transactionHash;
        if (!codedInputStream.ReadString(&transactionHash, SHA256HashSize)) {
            return false;
        }
    }
    return true;
}
