//
// Created by lhx on 1/21/21.
//

#ifndef MYBLOCKCHAIN_WORKER_H
#define MYBLOCKCHAIN_WORKER_H
class WorkerInstance;
class Worker{
public:
    explicit Worker(WorkerInstance* self):self(self){}
    ~Worker()= default;
    virtual void run() = 0;
    WorkerInstance* getWorkerInstance(){return self;}

protected:
    WorkerInstance* self;
};



#endif //MYBLOCKCHAIN_WORKER_H
