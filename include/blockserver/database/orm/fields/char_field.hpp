//
// Created by lhx on 1/24/22.
//

#ifndef MYBLOCKCHAIN_CHAR_FAILD_H
#define MYBLOCKCHAIN_CHAR_FAILD_H
#include "orm_filed.h"
namespace AriaORM {
    class CharField : public ORMFieldBase {
    public:
        CharField(std::string column_name, uint32_t max_length, bool nil = false)
                :ORMFieldBase(ORMFiledType::CharFiled, std::move(column_name)), maxLength(max_length) {
            this->setNull(nil);
        }

        uint32_t getMaxLength() { return maxLength; }
    private:
        uint32_t maxLength;
    };
}

#endif //MYBLOCKCHAIN_CHAR_FAILD_H
