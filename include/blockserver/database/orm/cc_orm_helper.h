
//
// Created by lhx on 1/24/22.
//

#ifndef MYBLOCKCHAIN_CC_ORM_HELPER_H
#define MYBLOCKCHAIN_CC_ORM_HELPER_H
#include <vector>
#include <string>
#include <memory>

class DBStorage;
class KVRWSet;

namespace AriaORM {
    class ORMInsert;
    class ORMQuery;
    class ORMTableDefinition;

    class CCORMHelper {
    public:
        CCORMHelper(uint32_t _tid, KVRWSet* _kvrwSet);
        ~CCORMHelper() {
            execute();
        }
        KVRWSet* getKVRWSet() { return kvrwSet; }

        AriaORM::ORMTableDefinition* createTable(const std::string& tableName);

        AriaORM::ORMQuery* newQuery(const std::string& tableName);

        AriaORM::ORMInsert* newInsert(const std::string& tableName);
    protected:
        uint32_t execute();
    private:
        uint32_t tid;
        DBStorage* storage = nullptr;
        std::vector<std::shared_ptr<AriaORM::ORMTableDefinition>> tableList;
        KVRWSet* kvrwSet;
        //没用
        std::vector<std::shared_ptr<AriaORM::ORMInsert>> insertList;
        std::vector<std::shared_ptr<AriaORM::ORMQuery>> queryList;
    };
}
#endif //MYBLOCKCHAIN_CC_ORM_HELPER_H
