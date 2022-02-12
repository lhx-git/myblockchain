//
// Created by lhx on 1/26/22.
//

#include "blockserver/worker/chaincode_stub_interface.h"
#include "blockserver/worker/impl/chaincode_object.h"
#include "blockserver/transaction/transaction.h"
#include "blockserver/database/orm/cc_orm_helper.h"

ChaincodeObject::ChaincodeObject(Transaction *transaction) {
    transactionID = transaction->getTransactionID();
    helper = new AriaORM::CCORMHelper(transactionID, transaction->getRWSet());
}

ChaincodeObject::~ChaincodeObject() {
    delete helper;
}

