
//
// Created by lhx on 1/25/22.
//

#ifndef MYBLOCKCHAIN_TABLE_IMPL_H
#define MYBLOCKCHAIN_TABLE_IMPL_H
#include "blockserver/database/orm/table_definition.h"
#include <vector>

namespace AriaORM {

    class ORMTableImpl: virtual public ORMTableDBDefinition{
    public:
        explicit ORMTableImpl(std::string _tableName): tableName(std::move(_tableName)) {}

        bool addField(const std::shared_ptr<ORMFieldBase>& fieldBase) override {
            fieldList.push_back(fieldBase);
            return true;
        }

        uint32_t fieldSize() override { return fieldList.size(); }

        ORMFieldBase* getField(uint32_t id) override {
            if(id >= fieldSize()) {
                return nullptr;
            } else {
                return fieldList[id].get();
            }
        }

        const std::string& getTableName() override { return tableName; }

        TableSetting* setTableSetting() override { return tableSetting; }

        const TableSetting* getSetting() override { return tableSetting; }

    private:
        std::vector<std::shared_ptr<ORMFieldBase>> fieldList;
        TableSetting* tableSetting{};
        const std::string tableName;
    };

}
#endif //MYBLOCKCHAIN_TABLE_IMPL_H
