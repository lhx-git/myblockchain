//
// Created by lhx on 2/7/22.
//

#include "common/sha256_hepler.h"
#include <iostream>
int main() {
    SHA256Helper *sha256Helper = new SHA256Helper();
    std::string data = "123456";
    std::string digest;
    sha256Helper->hash(data);
    sha256Helper->execute(&digest);
    //32字节 = 256位
    std::cout << digest.size() << std::endl;


    return 0;
}