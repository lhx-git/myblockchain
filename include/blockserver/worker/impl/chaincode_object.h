//
// Created by lhx on 1/26/22.
//

#ifndef MYBLOCKCHAIN_CHAINCODEOBJECT_H
#define MYBLOCKCHAIN_CHAINCODEOBJECT_H
#include <memory>
#include "blockserver/worker/chaincode_stub_interface.h"
class Transaction;
namespace AriaORM {
    class CCORMHelper;
}

class ChaincodeObject : public ChaincodeStubInterface {
public:
    explicit ChaincodeObject(Transaction* transaction);
    ~ChaincodeObject();

protected:
    virtual uint32_t InitFunc(std::vector<std::string>& args) = 0;

protected:
    AriaORM::CCORMHelper *helper;
    uint32_t transactionID;
};
#endif //MYBLOCKCHAIN_CHAINCODEOBJECT_H
