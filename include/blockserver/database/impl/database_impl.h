//
// Created by lhx on 1/25/22.
//

#ifndef MYBLOCKCHAIN_DATABASE_IMPL_H
#define MYBLOCKCHAIN_DATABASE_IMPL_H
#include "blockserver/database/database.h"
#include "blockserver/database/impl/level_db_storage.h"
#include "blockserver/database/impl/hash_map_storage.h"
#include "common/hash_map.h"
#include <memory>
//使用宏定义实现多态
//DatabaseImpl 没有重写DB_STORAGE_TYPE里面的任何一个方法，完全使用父类的方法，实现了所有VersionedDB接口方法。
class DatabaseImpl : public VersionedDB, DB_STORAGE_TYPE {
public:
    static VersionedDB* createInstance();

    DBStorage* getStorage() override;

    bool bufferUpdates(uint32_t tid, KVRWSet* kvRWSet,
                       std::vector<std::shared_ptr<AriaORM::ORMTableDefinition>> tableList) override;

    bool commitUpdate(uint32_t tid) override;
    bool commitUpdate(std::vector<Transaction *> transactionList) override;

    bool abortUpdate(uint32_t tid) override;

protected:
    void clearBufferThreadSafe();

private:
    DatabaseImpl();
    ~DatabaseImpl();

private:
    bool commitPhase;
    //<tid, rw set reference>
    HashMap<997, uint32_t, KVRWSet*> rwSetBuffer;
    //<tid, table list>
    HashMap<997, uint32_t, std::vector<std::shared_ptr<AriaORM::ORMTableDefinition>> > tableListBuffer;
    std::map<std::string, std::string> result;
};
#endif //MYBLOCKCHAIN_DATABASE_IMPL_H
