//
// Created by lhx on 1/19/22.
//

#ifndef MYBLOCKCHAIN_MOCK_RESERVE_TABLE_H
#define MYBLOCKCHAIN_MOCK_RESERVE_TABLE_H
#include "../reserve_table.h"
#include "common/hash_map.h"
#include <atomic>
class MockReserveTable : public ReserveTable {
public:
    MockReserveTable(uint32_t epoch);
    ~MockReserveTable() override;
    bool reserveRWset(KVRWSet *kvRWSet, uint32_t transactionID) override;
    TransactionDependency dependencyAnalysis(KVRWSet *kvRWSet, uint32_t transactionID) override;

private:
    //todo 认为这样也安全，而且更加省空间
    //std::map<std::string, HashMap<997, std::string, std::atomic<uint32_t>>> readTableList;
    //std::map<std::string, HashMap<997, std::string, std::atomic<uint32_t>>> writeTableList;
    HashMap<997, std::string, HashMap<997, std::string, std::atomic<uint32_t>>> readTableList; //表 ： key， tid
    HashMap<997, std::string, HashMap<997, std::string, std::atomic<uint32_t>>> writeTableList;

};
#endif //MYBLOCKCHAIN_MOCK_RESERVE_TABLE_H
