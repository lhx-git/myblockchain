//
// Created by lhx on 2/6/22.
//

#include "common/crypto_sign.h"
#include <iostream>
int main() {
    std::string password;
    CryptoSign::generateKeyFiles("/home/lhx/Desktop/pubKey", "/home/lhx/Desktop/priKey", reinterpret_cast<const unsigned char *>(password.data()), password.size());
    CryptoSign *testSign = new CryptoSign("/home/lhx/Desktop/pubKey", "/home/lhx/Desktop/priKey");
    std::string message = "hello";
    std::string signature;
    testSign->rsaEncrypt(message, &signature);

    std::cout << std::endl;
    if (testSign->rsaDecrypt(message, signature)) {
        std::cout << "correct" << std::endl;
    }
    return 0;
}
