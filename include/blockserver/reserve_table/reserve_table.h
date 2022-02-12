//
// Created by lhx on 1/19/22.
//

#ifndef MYBLOCKCHAIN_RESERVE_TABLE_H
#define MYBLOCKCHAIN_RESERVE_TABLE_H
#include <memory>
class KVRWSet;
class TransactionDependency;
class ReserveTable {
public:
    explicit ReserveTable(uint32_t epoch) : _epoch(epoch) {}
    virtual ~ReserveTable() = default;
    uint32_t getEpoch() const {return _epoch;}
    virtual bool reserveRWset(KVRWSet* kvRWSet, uint32_t transactionID) = 0;
    virtual TransactionDependency dependencyAnalysis(KVRWSet* kvRWSet, uint32_t transactionID) = 0;
private:
    uint32_t _epoch;
};

class ReserveTableProvider {
private:
    ReserveTableProvider() {};
public:
    static ReserveTableProvider* getHandle();
    ReserveTable* getReserveTable(uint32_t epoch);
    ReserveTable* getHashMapReserveTable(uint32_t epoch);
    ReserveTable* getMVCCReserveTable(uint32_t epoch);
private:
    std::shared_ptr<ReserveTable> reserveTable;
};
#endif //MYBLOCKCHAIN_RESERVE_TABLE_H
