//
// Created by lhx on 1/26/22.
//

#ifndef MYBLOCKCHAIN_ORM_TEST_CHAINCODE_H
#define MYBLOCKCHAIN_ORM_TEST_CHAINCODE_H
#include "blockserver/worker/impl/chaincode_object.h"
#include "common/random.h"
class ORMTestChaincode : public ChaincodeObject {
public:
    ORMTestChaincode(Transaction *transaction) : ChaincodeObject(transaction) {}
    uint32_t InvokeChaincode(std::string chaincodeName, std::vector<std::string>& args) override;

protected:
    uint32_t InitFunc(std::vector<std::string>& args) override;
protected:
    uint32_t createData(std::vector<std::string>& args);
private:
    Random random;
};
#endif //MYBLOCKCHAIN_ORM_TEST_CHAINCODE_H
