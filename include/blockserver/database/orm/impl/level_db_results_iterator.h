//
// Created by lhx on 1/25/22.
//

#ifndef MYBLOCKCHAIN_LEVEL_DB_ITERATOR_RESULT_H
#define MYBLOCKCHAIN_LEVEL_DB_ITERATOR_RESULT_H
#include "blockserver/database/orm/query.h"
class KVRWSet;

namespace AriaORM {
    class LevelDBResultsIterator : public ResultsIterator {
    public:
        explicit LevelDBResultsIterator(std::string _key, std::string _value);
        ~LevelDBResultsIterator() override;

        void init(KVRWSet* _kvRWSet, const std::string& _table) override;

        bool next() override;
        bool hasNext() override;

        int getInt(const std::string& attr) override;
        std::string getString(const std::string& attr) override;

    private:
        std::set<std::string> readKeys;
        std::string table;
        int index = 0;
        KVRWSet* kvRWSet{};
        const std::string key;
        const std::string value;
    };
}
#endif //MYBLOCKCHAIN_LEVEL_DB_ITERATOR_RESULT_H
