//
// Created by lhx on 1/26/22.
//

#ifndef MYBLOCKCHAIN_CHAINCODE_STUB_INTERFACE_H
#define MYBLOCKCHAIN_CHAINCODE_STUB_INTERFACE_H
#include <vector>
#include <string>

class ChaincodeStubInterface {
public:
    virtual uint32_t InvokeChaincode(std::string chaincodeName, std::vector<std::string>& args) = 0;
};
#endif //MYBLOCKCHAIN_CHAINCODE_STUB_INTERFACE_H
