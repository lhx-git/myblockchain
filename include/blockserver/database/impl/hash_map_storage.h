//
// Created by lhx on 1/25/22.
//

#ifndef MYBLOCKCHAIN_HASH_MAP_STORAGE_H
#define MYBLOCKCHAIN_HASH_MAP_STORAGE_H
#include "blockserver/database/db_storage.h"
#include "common/hash_map.h"


class HashMapStorage : public DBStorage {
public:
    HashMapStorage();
    void createTable(const std::string& tableName, const std::string& key, const std::string& value) override;
    AriaORM::ResultsIterator* selectDB(const std::string& key, const std::string& table) override;
    bool getTableStructure(const std::string& table, std::map<std::string, uint>& tableStructure) override;

    void removeWriteSet(const std::string &key, const std::string &value, const std::string &table) override;
    void updateWriteSet(const std::string& key, const std::string& value, const std::string& table) override;

private:
    HashMap<997, std::string, std::string> db;
    //Random random;
};
#endif //MYBLOCKCHAIN_HASH_MAP_STORAGE_H
