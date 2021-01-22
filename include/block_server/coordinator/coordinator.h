//
// Created by lhx on 1/21/21.
//

#ifndef MYBLOCKCHAIN_COORDINATOR_H
#define MYBLOCKCHAIN_COORDINATOR_H
#include "cstdlib"
#include <map>
class WorkerInstance;
class Config;
class Coordinator{
public:
    Coordinator(Config &config):config(config){};
    ~Coordinator() = default;
    virtual void run() = 0;

protected:
    Config &config;
    int32_t workerID;
    std::map<int32_t ,WorkerInstance*> workers;
};
#endif //MYBLOCKCHAIN_COORDINATOR_H
