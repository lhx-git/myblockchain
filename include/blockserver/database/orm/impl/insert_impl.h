//
// Created by lhx on 1/24/22.
//

#ifndef MYBLOCKCHAIN_INSERT_IMPL_H
#define MYBLOCKCHAIN_INSERT_IMPL_H
#include "blockserver/database/orm/insert.h"

class DBStorage;
class KVRWSet;
namespace AriaORM {
    class InsertImpl : public ORMInsert {
    public:
        InsertImpl(std::string _table, DBStorage* _storage, KVRWSet* _kvRWSet);
        ~InsertImpl();

        bool set(const std::string &attr, const std::string &value) override;
        bool set(const std::string &attr, int value) override;

    protected:
        void execute();

    private:
        std::string table;
        std::string key;
        DBStorage* dbStorage;
        KVRWSet* kvrwSet;
    };
}

#endif //MYBLOCKCHAIN_INSERT_IMPL_H
