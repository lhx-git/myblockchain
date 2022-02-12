//
// Created by lhx on 1/19/22.
//

#include "blockserver/reserve_table/mock/mock_reserve_table.h"
#include "kv_rwset.pb.h"
#include "blockserver/reserve_table/transaction_dependency.h"
MockReserveTable::MockReserveTable(uint32_t epoch) : ReserveTable(epoch) {

}

MockReserveTable::~MockReserveTable() {
    readTableList.clear();
    writeTableList.clear();
}

//读表中保留 key， 这一个epoch中第一个读key的交易的transactionID
//写表中保留 write， 这一个epoch中第一个写key的交易的transactionID
bool MockReserveTable::reserveRWset(KVRWSet *kvRWSet, uint32_t transactionID) {
    for(const auto& read : kvRWSet->reads())
    {   //TODO: for creating new table, add special key in reserve table (not modify code in here, but in kv read)
        HashMap<997, std::string, std::atomic<uint32_t>>& readTable = readTableList[read.table()];
        uint32_t old_rts;
        do {
            old_rts = readTable[read.key()];

            if (old_rts <= transactionID && old_rts != 0) {
                //if (old_rts < transactionID)
                //    DLOG(INFO) << "unable to reserve, previous:" << old_rts << ", new:" << transactionID;
                break;
            }
            // update rts
            // DLOG(INFO) << "read lock: " << read.key() << ", transaction: "<< transactionID << ", map_id: "<< readTable[read.key()];
            readTable[read.key()] = transactionID;
            // DLOG(INFO) << "read unlock: " << read.key() << ", transaction: "<< transactionID;
        } while (!readTable[read.key()].compare_exchange_weak(old_rts, transactionID));
    }

    for (const auto& write : kvRWSet->writes()) {
        HashMap<997, std::string, std::atomic<uint32_t>>& writeTable = writeTableList[write.table()];
        uint32_t old_wts;
        do {
            old_wts = writeTable[write.key()];

            if (old_wts <= transactionID && old_wts != 0) {
                //if (old_wts < transactionID)
                //    DLOG(INFO) << "unable to reserve, previous:" << old_wts << ", new:" << transactionID;
                break;
            }
            // update wts
            // DLOG(INFO) << "write lock: " << write.key() << ", transaction: "<< transactionID << ", map_id: "<< writeTable[write.key()];
            writeTable[write.key()] = transactionID;
            // DLOG(INFO) << "write unlock: " << write.key() << ", transaction: "<< transactionID;
        } while (!writeTable[write.key()].compare_exchange_weak(old_wts, transactionID));
    }
    return true;
}

TransactionDependency MockReserveTable::dependencyAnalysis(KVRWSet *kvRWSet, uint32_t transactionID) {
    TransactionDependency transactionDependency;
    //waw
    for (const auto &write : kvRWSet->writes()) {
        HashMap<997, std::string, std::atomic<uint32_t>>& writeTable = writeTableList[write.table()];
        uint32_t old_rts = writeTable[write.key()];
        if (old_rts != 0 && transactionID > old_rts) {
            transactionDependency.waw = true;
            // DLOG(INFO) << "waw, previous:" << old_rts << ", new:" << transactionID;
            break;
        }
    }
    //war
    for (const auto &write : kvRWSet->writes()) {
        HashMap<997, std::string, std::atomic<uint32_t>>& readTable = readTableList[write.table()];
        uint32_t old_rts = readTable[write.key()];
        if (old_rts != 0 && transactionID > old_rts) {
            transactionDependency.war = true;
            // DLOG(INFO) << "war, previous:" << old_rts << ", new:" << transactionID;
            break;
        }
    }
    //raw
    for (const auto &read : kvRWSet->reads()) {
        HashMap<997, std::string, std::atomic<uint32_t>>& writeTable = writeTableList[read.table()];
        uint32_t old_rts = writeTable[read.key()];
        if (old_rts != 0 &&  transactionID > old_rts) {
            // DLOG(INFO) << "raw, previous:" << old_rts << ", new:" << transactionID;
            transactionDependency.raw = true;
            break;
        }
    }
    return transactionDependency;
}
