//
// Created by lhx on 1/21/21.
//

#ifndef MYBLOCKCHAIN_ARIA_WORKER_H
#define MYBLOCKCHAIN_ARIA_WORKER_H
#include "worker.h"
#include "cstdlib"
#include "block_server/worker/worker_instance.h"

class AriaWorker : public Worker{
public:
    explicit AriaWorker(WorkerInstance* self):self(self){};
    ~AriaWorker()= default;;
    void run() override;
private:

    WorkerInstance* self;
};
#endif //MYBLOCKCHAIN_ARIA_WORKER_H
