//
// Created by lhx on 1/25/22.
//

#include "blockserver/database/orm/impl/level_db_results_iterator.h"
#include "blockserver/database/impl/level_db_storage.h"
#include "leveldb/db.h"
#include "glog/logging.h"

#define DB_FILENAME "/home/lhx/Desktop/testdb"

LevelDBStorage::LevelDBStorage() {
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, DB_FILENAME, &db);
    CHECK(status.ok());
}

void LevelDBStorage::createTable(const std::string &tableName, const std::string &key, const std::string &value) {
    LOG(INFO) << "level db does not support create table func.";
}

bool LevelDBStorage::getTableStructure(const std::string &table, std::map<std::string, uint> &tableStructure) {
    LOG(INFO) << "level db does not support get table structure func.";
    return true;
}

AriaORM::ResultsIterator *LevelDBStorage::selectDB(const std::string &key, const std::string &table) {
    std::string value;
    leveldb::Status status = db->Get(leveldb::ReadOptions(), key, &value);
    return new AriaORM::LevelDBResultsIterator(key, value);
}

void LevelDBStorage::updateWriteSet(const std::string &key, const std::string &value, const std::string &table) {
    leveldb::Status status = db->Put(leveldb::WriteOptions(), key, value);
    DCHECK(status.ok());
}

void LevelDBStorage::removeWriteSet(const std::string &key, const std::string &value, const std::string &table) {
    leveldb::Status status = db->Delete(leveldb::WriteOptions(), key);
    DCHECK(status.ok());
}