
//
// Created by lhx on 1/25/22.
//

#ifndef MYBLOCKCHAIN_LEVEL_DB_STORAGE_H
#define MYBLOCKCHAIN_LEVEL_DB_STORAGE_H
#include "blockserver/database/db_storage.h"

namespace leveldb{
    class DB;
}

class LevelDBStorage : public DBStorage {
public:
    LevelDBStorage();
    void createTable(const std::string& tableName, const std::string& key, const std::string& value) override;
    //没用，leveldb无表结构
    bool getTableStructure(const std::string& table, std::map<std::string, uint>& tableStructure) override;
    //读操作
    AriaORM::ResultsIterator* selectDB(const std::string& key, const std::string& table) override;
    //删除操作
    void removeWriteSet(const std::string &key, const std::string &value, const std::string &table) override;
    //更新操作
    void updateWriteSet(const std::string& key, const std::string& value, const std::string& table) override;

private:
    leveldb::DB* db{};
};
#endif //MYBLOCKCHAIN_LEVEL_DB_STORAGE_H
