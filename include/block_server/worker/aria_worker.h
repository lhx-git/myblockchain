//
// Created by lhx on 1/21/21.
//

#ifndef MYBLOCKCHAIN_ARIA_WORKER_H
#define MYBLOCKCHAIN_ARIA_WORKER_H
#include "worker.h"
#include "cstdlib"
class AriaWorker : public Worker{
public:
    explicit AriaWorker(u_int32_t workerID):workerID(workerID){};
    ~AriaWorker()= default;;
    void run() override;
private:
    u_int32_t workerID;
};
#endif //MYBLOCKCHAIN_ARIA_WORKER_H
