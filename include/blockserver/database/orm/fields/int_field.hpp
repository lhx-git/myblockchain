//
// Created by lhx on 1/24/22.
//

#ifndef MYBLOCKCHAIN_INT_FIELD_H
#define MYBLOCKCHAIN_INT_FIELD_H

#include "orm_filed.h"
namespace AriaORM {
    class IntField : public ORMFieldBase {
    public:
        IntField(ORMFiledType _ormFiledType, std::string _columnName, uint32_t _maxLength) :
                ORMFieldBase(_ormFiledType, _columnName) {}
    };
}
#endif //MYBLOCKCHAIN_INT_FIELD_H
