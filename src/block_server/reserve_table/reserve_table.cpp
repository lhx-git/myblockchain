//
// Created by lhx on 1/20/22.
//

#include "blockserver/reserve_table/mock/mock_reserve_table.h"
#include "blockserver/reserve_table/reserve_table.h"
#include <mutex>
//线程安全的单例模式
ReserveTableProvider *ReserveTableProvider::getHandle() {
    static ReserveTableProvider* reserveTableProvider = nullptr;
    static std::mutex mutex;
    if (reserveTableProvider == nullptr) {//执行
        std::unique_lock<std::mutex> lock(mutex);
        if (reserveTableProvider == nullptr) {//防止重入
            reserveTableProvider = new ReserveTableProvider;
        }
    }
    return reserveTableProvider;
}

ReserveTable *ReserveTableProvider::getReserveTable(uint32_t epoch) {
    return GET_RESERVE_TABLE_TYPE(epoch);
}

ReserveTable *ReserveTableProvider::getHashMapReserveTable(uint32_t epoch) {
    if (reserveTable == nullptr || reserveTable->getEpoch() != epoch) {
        static std::mutex mutex;
        std::unique_lock<std::mutex> lock(mutex);
        if (reserveTable == nullptr || reserveTable->getEpoch() != epoch) {
            reserveTable = std::make_shared<MockReserveTable>(epoch);
        }
    }
    return reserveTable.get();
}

ReserveTable *ReserveTableProvider::getMVCCReserveTable(uint32_t epoch) {
    return nullptr;
}


