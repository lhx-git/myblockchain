//
// Created by lhx on 1/20/22.
//

#ifndef MYBLOCKCHAIN_DB_STORAGE_H
#define MYBLOCKCHAIN_DB_STORAGE_H
#include <string>
#include <map>
#include <memory>
#include <vector>

namespace AriaORM {
    class ResultsIterator;
    class ORMTableDefinition;
}

class KVRWSet;
class DBStorage {
public:
    virtual void createTable(const std::string& tableName, const std::string& key, const std::string& value) = 0;
    // this func new AriaORM::ResultsIterator, which must be deleted later.
    virtual AriaORM::ResultsIterator* selectDB(const std::string& key, const std::string& table) = 0;
    virtual bool getTableStructure(const std::string& table, std::map<std::string, uint>& tableStructure) = 0;


    virtual void removeWriteSet(const std::string& key, const std::string& value, const std::string& table) = 0;
    virtual void updateWriteSet(const std::string& key, const std::string& value, const std::string& table) = 0;

    // this func is impl in database impl
    virtual bool bufferUpdates(uint32_t tid, KVRWSet* kvRWSet, std::vector<std::shared_ptr<AriaORM::ORMTableDefinition>> tableList) = 0;
};
#endif //MYBLOCKCHAIN_DB_STORAGE_H
