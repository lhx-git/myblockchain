//
// Created by lhx on 1/21/21.
//

#ifndef MYBLOCKCHAIN_ARIA_WORKER_H
#define MYBLOCKCHAIN_ARIA_WORKER_H
#include "worker.h"
#include "block_server/worker/worker_instance.h"

class AriaWorker : public Worker{
public:
    AriaWorker(WorkerInstance *self):Worker(self){}
    ~AriaWorker()= default;

    [[noreturn]] void run() override;
private:

};

#endif //MYBLOCKCHAIN_ARIA_WORKER_H
