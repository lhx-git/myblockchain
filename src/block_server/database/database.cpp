//
// Created by lhx on 1/25/22.
//

#include "blockserver/database/database.h"
#include "blockserver/database/impl/database_impl.h"
#include <mutex>

VersionedDB *VersionedDB::getDBInstance() {
    static VersionedDB* db = nullptr;
    static std::mutex mutex;
    if (db == nullptr) {//执行
        std::unique_lock<std::mutex> lock(mutex);
        if (db == nullptr) {//防止重入
            db = DatabaseImpl::createInstance();
        }
    }
    return db;
}