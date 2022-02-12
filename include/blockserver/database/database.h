//
// Created by lhx on 1/20/22.
//

#ifndef MYBLOCKCHAIN_DATABASE_H
#define MYBLOCKCHAIN_DATABASE_H
#include <vector>
#include <memory>
class Transaction;
class DBStorage;

//VersionedDB定义了一个数据库对象需要实现的方法
class VersionedDB {
public:
    static VersionedDB* getDBInstance();
    virtual DBStorage* getStorage() = 0;

    virtual bool commitUpdate(uint32_t tid) = 0;
    virtual bool commitUpdate(std::vector<Transaction*> transactionList) = 0;

    virtual bool abortUpdate(uint32_t tid) = 0;
};
#endif //MYBLOCKCHAIN_DATABASE_H
