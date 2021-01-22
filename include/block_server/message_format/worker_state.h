//
// Created by peng on 2021/1/16.
//

#ifndef NEUBLOCKCHAIN_WORKER_STATE_H
#define NEUBLOCKCHAIN_WORKER_STATE_H

// READY means no task
// ASSIGNED means processing data
// EXITED means can be free from memory
// WORKER STATE CAN BE SET BY COORDINATOR OR WORKER BOTH!
enum class WorkerState {
        READY,          //worker is initializing or is free
        ASSIGNED,       // worker has init and is ready to process data
        FINISH_READ,    // worker finish read phase
        FINISH_COMMIT,  // expired, same as ready
        EXITED          // worker is about to exit, use join func to safe delete it.
};

#endif //NEUBLOCKCHAIN_WORKER_STATE_H
