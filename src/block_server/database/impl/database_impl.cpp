//
// Created by lhx on 1/26/22.
//

#include "blockserver/database/impl/database_impl.h"
#include "blockserver/database/db_storage.h"
#include "blockserver/reserve_table/mock/mock_reserve_table.h"

#include "blockserver/database/orm/table_definition.h"
#include "blockserver/database/orm/fields/char_field.hpp"
#include "blockserver/transaction/transaction.h"

#include "glog/logging.h"
#include "kv_rwset.pb.h"
VersionedDB *DatabaseImpl::createInstance() {
    return new DatabaseImpl();
}


DatabaseImpl::DatabaseImpl()
        :commitPhase(false) {

}

DatabaseImpl::~DatabaseImpl() {

}

bool DatabaseImpl::bufferUpdates(uint32_t tid, KVRWSet* kvRWSet,
                                 std::vector<std::shared_ptr<AriaORM::ORMTableDefinition>> tableList) {
    // notice: re-enter protection
    // all the worker has one unique coordinator, so it does not happen when one worker is
    // committing while other is buffering updates.
    //clearBuffer函数在 下个epoch的第一个交易填充buffer之前被调用
    if(commitPhase)
       clearBufferThreadSafe();
    // DLOG(INFO) << tid <<"write";
    rwSetBuffer[tid] = kvRWSet;
    tableListBuffer[tid] = tableList;
    return true;
}

bool DatabaseImpl::commitUpdate(uint32_t tid) {
    static std::mutex mutex;
    if(!commitPhase) {  // performance
        std::unique_lock<std::mutex> lock(mutex);
        if(!commitPhase) {  // prevent for re-enter
            DLOG(INFO) << "update buffer to db";
            DCHECK(rwSetBuffer.contains(tid));
            DCHECK(tableListBuffer.contains(tid));
            // first, create table， 使用leveldb不需要创建表
//            for (const auto& _table: tableListBuffer[tid]) {
//                auto* table = dynamic_cast<AriaORM::ORMTableDBDefinition*>(_table.get());
//                CHECK(table->fieldSize() == 2);
//                this->createTable(table->getTableName(),
//                                  table->getField(0)->getColumnName(),
//                                  table->getField(1)->getColumnName());
//            }
            // then, update writes
            for(const KVWrite& kvWrite: rwSetBuffer[tid]->writes()) {
                if (kvWrite.is_delete()) {
                    removeWriteSet(kvWrite.key(), " ", kvWrite.table());
                } else {
                    updateWriteSet(kvWrite.key(), kvWrite.value(), kvWrite.table());
                }
            }
            commitPhase = true;    // move flag to last, thread safe.
        }
    }
    return true;
}

//todo commitupdate(transactionList)方法有问题，只能提交一个worker的交易。
//这个函数是为了实现CRDT的Merge而写的
bool DatabaseImpl::commitUpdate(std::vector<Transaction *> transactionList) {
    static std::mutex mutex;
    if(!commitPhase) {  // performance
        std::unique_lock<std::mutex> lock(mutex);
        if(!commitPhase) {  // prevent for re-enter
            DLOG(INFO) << "update buffer to db";
            for (auto tr : transactionList) {
                for (const KVRead& kvRead : rwSetBuffer[tr->getTransactionID()]->reads()) {
                    //LOG(INFO) << "read set key: " << kvRead.key() << " value: " << kvRead.value();
                    if (!result.count(kvRead.key())) {
                        if(kvRead.value() == "") {
                            result[kvRead.key()] = "0";
                            continue;
                        }
                        result[kvRead.key()] = kvRead.value();
                    }
                }
                for (const KVWrite& kvWrite : rwSetBuffer[tr->getTransactionID()]->writes()) {
                    if (!result.count(kvWrite.key())) {
                        result[kvWrite.key()] = "0";
                    }
                    result[kvWrite.key()] = std::to_string(std::stoi(result[kvWrite.key()]) + std::stoi(kvWrite.value()));
                }
            }
            auto iter = result.begin();
            while (iter != result.end()) {
                //LOG(INFO) << "store to db key: " << iter->first << " value: " << iter->second;
                updateWriteSet(iter->first, iter->second, "test_table");
                iter++;
            }
            result.clear();
            commitPhase = true;    // move flag to last, thread safe.
        }
    }
    return true;
}

bool DatabaseImpl::abortUpdate(uint32_t tid) {
    return false;
}

//在用一个新epoch的交易填充buffer之前,调用此函数,清空上一个epoch产生的buffer
void DatabaseImpl::clearBufferThreadSafe() {
    static std::mutex mutex;
    if(commitPhase) {  // performance
        std::unique_lock<std::mutex> lock(mutex);
        if(commitPhase) {  // prevent for re-enter
            DLOG(INFO) << "clear buffer";
            rwSetBuffer.clear();
            tableListBuffer.clear();
            commitPhase = false;    // move flag to last, thread safe.
        }
    }
}

DBStorage *DatabaseImpl::getStorage() {
    return this;
}
