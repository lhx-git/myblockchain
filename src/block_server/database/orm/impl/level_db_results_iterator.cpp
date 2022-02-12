//
// Created by lhx on 1/25/22.
//

#include "blockserver/database/orm/impl/level_db_results_iterator.h"
#include "kv_rwset.pb.h"

AriaORM::LevelDBResultsIterator::LevelDBResultsIterator(std::string _key, std::string _value) : key(std::move(_key)), value(std::move(_value)) {

}


//index = 2, haxNext = -1, index = 3;
//index = 3, hasNext = -2, index = 4;

//index = 0 hasNext = 1, index = 1;
//index = 1, hasNext = 0 return false;
//todo 这里逻辑有问题， 只有当index = 0时，才运行正确

bool AriaORM::LevelDBResultsIterator::next() {
    if(hasNext())
        return ++index;
    return false;
}

void AriaORM::LevelDBResultsIterator::init(KVRWSet *_kvRWSet, const std::string &_table) {
    kvRWSet = _kvRWSet; table = _table;
}

bool AriaORM::LevelDBResultsIterator::hasNext() {
    return 1 - index;
}

int AriaORM::LevelDBResultsIterator::getInt(const std::string& attr) {
    return 0;
}

//设置读集
std::string AriaORM::LevelDBResultsIterator::getString(const std::string& attr) {
    if(readKeys.count(key) == 0) {
        // prevent for get the same key
        KVRead* read = kvRWSet->add_reads();
        read->set_table(table);
        // notice: this key represent the value of the given attribute
        read->set_key(key);
        //todo: lhx modify here
        read->set_value(value);
        readKeys.insert(key);
    }
    if(attr == "key") {
        return key;
    }
    return value;
}

AriaORM::LevelDBResultsIterator::~LevelDBResultsIterator()  {

}