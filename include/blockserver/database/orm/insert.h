//
// Created by lhx on 1/24/22.
//

#ifndef MYBLOCKCHAIN_INSERT_H
#define MYBLOCKCHAIN_INSERT_H
#include <string>
namespace AriaORM {
    class ORMInsert {
    public:
        virtual bool set(const std::string &attr, const std::string &value) = 0;
        virtual bool set(const std::string &attr, int value) = 0;
    };
}
#endif //MYBLOCKCHAIN_INSERT_H
