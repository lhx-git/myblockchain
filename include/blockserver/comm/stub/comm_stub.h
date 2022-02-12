//
// Created by peng on 2021/1/22.
//

#ifndef NEUBLOCKCHAIN_COMMSTUB_H
#define NEUBLOCKCHAIN_COMMSTUB_H

#include "blockserver/comm/comm.h"
#include "tpc-c.pb.h"
#include "common/random.h"

#include <map>
#include <vector>

class Transaction;

class CommStub: public Comm{
public:
    CommStub();
    TransactionWrapper* getTransaction(uint32_t epoch, uint32_t maxSize, uint32_t minSize) override;
    void clearTransactionForEpoch(uint32_t epoch) override;

private:
    uint32_t epoch;
    // for restore test, this need to be changed as well.
    // because the impl of this dont need epoch, the creation of trs, which is the usage of epoch, belongs to client proxy
    uint32_t tidNonce = 1;
    uint32_t epochTrSentNum = 0;
    std::map<uint32_t ,std::vector<Transaction*>> transactionMap;

private:
    Random randomHelper;
    YCSB_PAYLOAD ycsbPayload;
    CRDT_GCOUNTER_PAYLOAD crdtGcounterPayload;

    const uint32_t EPOCH_MAX_TR_NUM = 110;
    const int readSize = 2;
    const int writeSize = 4 - readSize;
};

#endif //NEUBLOCKCHAIN_COMMSTUB_H
