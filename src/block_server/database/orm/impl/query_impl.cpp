//
// Created by lhx on 1/25/22.
//

#include "blockserver/database/orm/impl/query_impl.h"
#include "blockserver/database/orm/query.h"
#include "blockserver/database/db_storage.h"

AriaORM::QueryImpl::QueryImpl(std::string _table, DBStorage *_storage, KVRWSet *_kvRWSet) :
    table(_table), storage(_storage), kvRWSet(_kvRWSet) {

}

void AriaORM::QueryImpl::filter(const std::string &attr, NumFilter range, int arg1, int arg2) {

}

void AriaORM::QueryImpl::filter(const std::string &attr, NumFilter range, double arg1, double arg2) {

}

void AriaORM::QueryImpl::filter(const std::string &attr, StrFilter range, const std::string &arg) {
    this->col = arg;
}

void AriaORM::QueryImpl::orderBy(std::vector<std::string> orderAttr) {

}

std::string AriaORM::QueryImpl::raw() const {
    return std::string();
}

AriaORM::ResultsIterator *AriaORM::QueryImpl::executeQuery(const std::string &query) {
    return nullptr;
}

AriaORM::ResultsIterator *AriaORM::QueryImpl::executeQuery() {
    delete iter;
    iter = storage->selectDB(col, table);
    iter->init(kvRWSet, table);
    return iter;
}

AriaORM::QueryImpl::~QueryImpl() {
    delete iter;
}