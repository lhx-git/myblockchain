//
// Created by lhx on 1/21/21.
//

#ifndef MYBLOCKCHAIN_WORKER_H
#define MYBLOCKCHAIN_WORKER_H
class Worker{
public:
    Worker()=default;
    ~Worker(){};
    virtual void run() = 0;
};
#endif //MYBLOCKCHAIN_WORKER_H
