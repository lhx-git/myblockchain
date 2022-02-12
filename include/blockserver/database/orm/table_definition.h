//
// Created by lhx on 1/24/22.
//

#ifndef MYBLOCKCHAIN_TABLE_DEFINATION_H
#define MYBLOCKCHAIN_TABLE_DEFINATION_H
#include <string>
#include <memory>

namespace AriaORM {
    class ORMFieldBase;
    class TableSetting;

    class ORMTableDefinition {
    public:
        virtual bool addField(const std::shared_ptr<ORMFieldBase>& fieldBase) = 0;
        virtual TableSetting* setTableSetting() = 0;
    };

    // this interface is use for database_impl. commit update.
    class ORMTableDBDefinition: virtual public ORMTableDefinition {
    public:
        virtual uint32_t fieldSize() = 0;
        virtual ORMFieldBase* getField(uint32_t id) = 0;
        virtual const std::string& getTableName() = 0;
        virtual const TableSetting* getSetting() = 0;
    };
}
#endif //MYBLOCKCHAIN_TABLE_DEFINATION_H
