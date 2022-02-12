//
// Created by lhx on 1/26/22.
//

#include "blockserver/worker/impl/orm_test_chaincode.h"
#include "glog/logging.h"

#include "blockserver/database/orm/cc_orm_helper.h"
#include "blockserver/database/orm/table_definition.h"
#include "blockserver/database/orm/query.h"
#include "blockserver/database/orm/insert.h"
#include "blockserver/database/orm/fields/char_field.hpp"
#include "tpc-c.pb.h"


#define YCSB_FIELD_SIZE 10
#define KEY_PER_PA 200000

uint32_t ORMTestChaincode::InvokeChaincode(std::string chaincodeName, std::vector<std::string> &args) {
    if(chaincodeName == "correct_test") {
        AriaORM::ORMQuery *query = helper->newQuery("test_table");
        query->filter("key", AriaORM::StrFilter::EQU, args[0]);
        for (AriaORM::ResultsIterator *r = query->executeQuery(); r->hasNext(); r->next()) {
            //auto add read set in here
            LOG(INFO) << this->transactionID <<" READ: (" << r->getString("key") <<", " <<  r->getString("value") << ")";
        }

        AriaORM::ORMInsert* insert = helper->newInsert("test_table");
        std::string value = random.a_string(YCSB_FIELD_SIZE, YCSB_FIELD_SIZE);
        insert->set("key", args[0]);
        insert->set("value", value);
        LOG(INFO) << this->transactionID <<" INSERT: (" << args[0] <<", " <<  value << ")";
        return true;
    }

    if(chaincodeName == "create_table") {
        return InitFunc(args);
    }

    if(chaincodeName == "create_data") {
        return createData(args);
    }
    if (chaincodeName == "crdt_gcounter") {
        int temp_val = 0;
        CRDT_GCOUNTER_PAYLOAD crdtGcounterPayload;
        /*crdtGcounterPayload.set_key("chw");
        crdtGcounterPayload.set_value(1);*/
        crdtGcounterPayload.ParseFromString(args[0]);
        //1.query
        AriaORM::ORMQuery *query = helper->newQuery("test_table");
        query->filter("key", AriaORM::StrFilter::EQU, crdtGcounterPayload.key());
        //LOG(INFO) << "key receive from client is:" << crdtGcounterPayload.key();
        for (AriaORM::ResultsIterator *r = query->executeQuery(); r->hasNext(); r->next()) {
            r->getString("key");//generate read set but peng's getString method don't add value to read set
            r->getString("value");
            //LOG(INFO) << "key search from db is:" << r->getString("key") << "value:" << r->getString("value");
        }

        AriaORM::ORMInsert* insert = helper->newInsert("test_table");
        insert->set("key",crdtGcounterPayload.key());
        insert->set("value", std::to_string(crdtGcounterPayload.value()));
        return true;
    }

    if(chaincodeName == "mixed") {
        YCSB_PAYLOAD ycsbPayload;
        ycsbPayload.ParseFromString(args[0]);

        for (auto& write: ycsbPayload.update()) {
            AriaORM::ORMInsert* insert = helper->newInsert("test_table");
            insert->set("key", write);
            insert->set("value", random.a_string(YCSB_FIELD_SIZE, YCSB_FIELD_SIZE));
        }

        for (auto& read: ycsbPayload.reads()) {
            //table: test_table, pk column: test
            AriaORM::ORMQuery *query = helper->newQuery("test_table");
            query->filter("key", AriaORM::StrFilter::EQU, read);

            for (AriaORM::ResultsIterator *r = query->executeQuery(); r->hasNext(); r->next()) {
                //auto add read set in here
                r->getString("key");
                r->getString("value");
            }
        }
        return true;
    }
    return false;
}

uint32_t ORMTestChaincode::InitFunc(std::vector<std::string> &args) {
    AriaORM::ORMTableDefinition* table = helper->createTable("test_table");
    std::shared_ptr<AriaORM::CharField> key_charField = std::make_shared<AriaORM::CharField>("key", 200, true);
    table->addField(key_charField);
    std::shared_ptr<AriaORM::CharField> value_charField = std::make_shared<AriaORM::CharField>("value", 200, true);
    table->addField(value_charField);
    return true;
    /*AriaORM::ORMTableDefinition* table = helper->createTable("test_table");
    // first, init primary key attr, this field is int field,
    // (attribute name: key, can be nil: false).
    std::shared_ptr<AriaORM::IntField> intField =
            std::make_shared<AriaORM::IntField>("key", false);
    intField->setPrimary(true);
    table->addField(intField);
    // then, init value attr, this field is char(string) field,
    // (attribute name: value, max_length is 200, can be nil: false).
    std::shared_ptr<AriaORM::CharField> charField =
            std::make_shared<AriaORM::CharField>("value", 200, true);
    table->addField(charField);
    // return true to commit this transaction.
    return true;*/
}

uint32_t ORMTestChaincode::createData(std::vector<std::string> &args) {

    /*for gcounter*/
    AriaORM::ORMInsert* insert = helper->newInsert("test_table");
    insert->set("key", "lhx");
    insert->set("value", "10");
    insert->set("key", "chw");
    insert->set("value", "20");
    insert->set("key", "wyt");
    insert->set("value", "30");
    /*
    for(int i = 0; i < KEY_PER_PA; i++) {
        AriaORM::ORMInsert* insert = helper->newInsert("test_table");
        insert->set("key", std::to_string(i));
        insert->set("value", random.a_string(YCSB_FIELD_SIZE, YCSB_FIELD_SIZE));
    }*/

    return true;
}
