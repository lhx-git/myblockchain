//
// Created by lhx on 1/25/22.
//

#include "blockserver/database/orm/cc_orm_helper.h"
#include "blockserver/database/database.h"
#include "blockserver/database/orm/impl/table_impl.h"
#include "blockserver/database/orm/impl/query_impl.h"
#include "blockserver/database/orm/impl/insert_impl.h"
#include "blockserver/database/db_storage.h"
#include "kv_rwset.pb.h"
#include "glog/logging.h"
#include "common/complie_config.h"

AriaORM::CCORMHelper::CCORMHelper(uint32_t _tid,  KVRWSet *_kvrwSet) : tid(_tid) {
    kvrwSet = _kvrwSet;
    storage = VersionedDB::getDBInstance()->getStorage();
    DCHECK(storage);
    DCHECK(kvrwSet->reads().empty());
    DCHECK(kvrwSet->writes().empty());
}

AriaORM::ORMTableDefinition *AriaORM::CCORMHelper::createTable(const std::string &tableName) {
    std::shared_ptr<AriaORM::ORMTableImpl> table = std::make_shared<AriaORM::ORMTableImpl>(tableName);
    KVWrite* kvWrite = kvrwSet->add_writes();
    kvWrite->set_table(ARIA_SYS_TABLE);
    kvWrite->set_key(tableName);
    kvWrite->set_value("place holder of " + std::to_string(tid));
    tableList.push_back(table);
    return table.get();
}

//一笔交易对应一个helper对象
//newQuery 读数据库，构造KVRWSet的读集
//newInsert 构造KVRWSet的写集
//execute方法，将{tid， kvRwSet，tableList} 传入db对象中


//一个kvrwset指针，被多个对象设置，在哪里被初始化呢？ 在哪里被释放呢？
//回答：一个交易一个kvrwset， 应该在交易初始化的时候，初始化一个空的kvrwset，然后在释放交易的时候，将kvrwset释放
AriaORM::ORMQuery *AriaORM::CCORMHelper::newQuery(const std::string &tableName) {
    std::shared_ptr<AriaORM::QueryImpl> query = std::make_shared<AriaORM::QueryImpl>(tableName, storage, kvrwSet);
    queryList.push_back(query);
    return query.get();
}

AriaORM::ORMInsert *AriaORM::CCORMHelper::newInsert(const std::string &tableName) {
    std::shared_ptr<AriaORM::InsertImpl> insert = std::make_shared<AriaORM::InsertImpl>(tableName, storage, kvrwSet);
    insertList.push_back(insert);
    return insert.get();
}


uint32_t AriaORM::CCORMHelper::execute() {
    storage->bufferUpdates(tid, kvrwSet, tableList);
    return true;
}
