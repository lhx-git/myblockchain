//
// Created by lhx on 1/25/22.
//

#include "blockserver/database/orm/impl/insert_impl.h"
#include "kv_rwset.pb.h"

AriaORM::InsertImpl::InsertImpl(std::string _table, DBStorage *_storage, KVRWSet *_kvRWSet) :
    table(std::move(_table)), dbStorage(_storage), kvrwSet(_kvRWSet) {

}

bool AriaORM::InsertImpl::set(const std::string &attr, const std::string &value) {
    //设置insert对象的key
    if (attr == "key") {
        key = value;
        return true;
    }
    //将key，value，加入写集
    if (attr == "value") {
        KVWrite* write = kvrwSet->add_writes();
        write->set_key(key);
        write->set_value(value);
        write->set_table(table);
        return true;
    }
    return false;
}


bool AriaORM::InsertImpl::set(const std::string &attr, int value) {
    return false;
}

void AriaORM::InsertImpl::execute() {

}

AriaORM::InsertImpl::~InsertImpl() {
    execute();
}


