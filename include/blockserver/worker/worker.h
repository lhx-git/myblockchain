//
// Created by lhx on 1/13/22.
//

#pragma once
#include "worker_instance.h"
#ifndef MYBLOCKCHAIN_WORKER_H
#define MYBLOCKCHAIN_WORKER_H
class Worker {
public:
    Worker(WorkerInstance* self) : _self(self) {}
    virtual void run() = 0;
    virtual ~Worker() = default;

    inline WorkerInstance* getInstance() {
        return _self;
    }
protected:
    WorkerInstance* _self;
};
#endif //MYBLOCKCHAIN_WORKER_H
