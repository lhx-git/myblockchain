//
// Created by lhx on 1/13/22.
//
#ifndef MYBLOCKCHAIN_WORKER_INSTANCE_H
#define MYBLOCKCHAIN_WORKER_INSTANCE_H

#include <atomic>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
enum class WorkerState;
class Worker;
class Workload;
class WorkerInstance {
public:
    explicit WorkerInstance(std::atomic<uint32_t>& ep) : epochChan(ep) {

    }
    ~WorkerInstance() = default;
    std::atomic<uint32_t> workerID;

    std::atomic<uint32_t>& epochChan;

    std::atomic<uint32_t> ariaGlobalState;
    std::atomic<uint32_t> workerState;
    std::shared_ptr<std::thread> thread;
    std::shared_ptr<Worker> instance;
    std::shared_ptr<Workload> workload;

    //Coordinator线程在syncAllWorkerState时，如果有工作线程还没执行完成时，调用produce.wait()，将自己阻塞
    //worker线程调用,用来唤醒Coordinator
    inline void notifyPruduce() {
        produce.notify_one();
    }

    //Worker线程在的等待Coordinator信号时waitForSignal，如果等待的信号没到达时，调用consume.wait() 将自己阻塞
    //Coordinator线程调用
    inline void notifyConsume() {
        consume.notify_one();
    }
public:
    std::mutex mutex;
    std::condition_variable produce, consume;
};
#endif //MYBLOCKCHAIN_WORKER_INSTANCE_H
