//
// Created by lhx on 2021/1/16.
//

#ifndef NEUBLOCKCHAIN_WORKER_STATE_H
#define NEUBLOCKCHAIN_WORKER_STATE_H

// READY 表示没有任务
// ASSIGNED 表示正在处理数据
// EXITED 表示该worker可以从内内存中释放了
// worker状态 既可以被Coordinator设置，也可以被Worker设置!
enum class WorkerState {
        READY,          //worker is initializing or is free
        ASSIGNED,       // worker已经初始化完毕并且正准备去处理数据
        FINISH_READ,    // worker完成了读阶段
        FINISH_COMMIT,  // 已经弃用，等同于Ready
        EXITED          // worker将要被废弃掉，用join函数来安全的删除
};

#endif //NEUBLOCKCHAIN_WORKER_STATE_H
