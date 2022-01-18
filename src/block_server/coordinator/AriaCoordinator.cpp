//
// Created by lhx on 1/14/22.
//

#include "blockserver/worker/worker_instance.h"
#include "blockserver/coordinator/AriaCoordinator.h"
#include "blockserver/message_fomat/aria_global_state.h"
#include "blockserver/message_fomat/worker_state.h"
#include "blockserver/message_fomat/workload.h"
#include "blockserver/worker/AriaWorker.h"
#include <thread>
#include "blockserver/transaction/transaction.h"
#include "glog/logging.h"
#include "../common/complie_config.h"

AriaCoordinator::AriaCoordinator(std::atomic<uint32_t> &epochChan) : Coordinator(epochChan) {
    globalState = AriaGlobalState::START;
    this->IDNounce = 1;
    broadcastToAllWorker();
}

AriaCoordinator::~AriaCoordinator() {
    globalState = AriaGlobalState::EXIT;
    broadcastToAllWorker(false);
    syncAllWorker(WorkerState::EXITED, false);
    destroyUnusedWorkers();
    LOG(INFO) << destroyUnusedWorkers() << " worker has stopped, exit coordinator." << std::endl;
}

void AriaCoordinator::run() {
    while (true) {
        mainRoutine();
    }
}

void AriaCoordinator::mainRoutine() {
    globalState = AriaGlobalState::START;
    broadcastToAllWorker(false);

    std::shared_ptr<Workload> workload = nullptr;
    uint32_t isCurrentEpochEmpty = false;
    while (getWorkload(isCurrentEpochEmpty, workload), !isCurrentEpochEmpty) {
        uint32_t workerId = deployWorkload(std::move(workload));
        if (workerId == -1) {
            LOG(ERROR) << "deploy workload failed! worker status:";
            printWorkerCondition();
        }
        setWorkerState(workerId, AriaGlobalState::Aria_READ);
    }

    DLOG(INFO) << "start aria protocol, epoch = " << this->epochChan;
    globalState = AriaGlobalState::Aria_READ;
    broadcastToAllWorker();
    syncAllWorker(WorkerState::FINISH_READ);

    globalState = AriaGlobalState::Aria_COMMIT;
    broadcastToAllWorker();
    syncAllWorker(WorkerState::READY);

    //一个epoch的交易都处理完成
    DLOG(INFO) << "all worker, num: "<< this->workers.size() <<" finished epoch = " << this->epochChan << ", write back data.";
    // announce the manager
    epochTransactionFinishSignal(epochChan);
    //update the epoch finally
    epochChan += 1;
}

uint32_t AriaCoordinator::createWorker(std::shared_ptr<Workload> workload) {
    WorkerInstance* worker = new WorkerInstance(epochChan);
    worker->instance = std::make_shared<AriaWorker>(worker);
    worker->workload = std::move(workload);
    worker->ariaGlobalState = static_cast<uint32_t>(globalState);
    worker->workerID = IDNounce;
    workers[worker->workerID] = worker;

    IDNounce++;
    //运行线程
    worker->thread = std::make_shared<std::thread>(&Worker::run, worker->instance.get());
    syncWorkerState(worker->workerID, WorkerState::ASSIGNED);//worker状态机 是从Assigned开始
    return worker->workerID;
}


void AriaCoordinator::syncAllWorker(WorkerState state, bool onlyAssigned) {
    for (auto worker : workers) {
        std::unique_lock<std::mutex> produceLock(worker.second->mutex);
        if (onlyAssigned && worker.second->workerState.load() == static_cast<uint32_t>(WorkerState::READY)) continue;
        while (worker.second->workerState.load() != static_cast<uint32_t>(state)) {
            worker.second->produce.wait(produceLock);
        }
    }
}

void AriaCoordinator::setWorkerState(int workerID, AriaGlobalState state) {
    if (!workers.count(workerID)) {
        return ;
    }
    std::unique_lock<std::mutex> produceLock(workers[workerID]->mutex);
    workers[workerID]->ariaGlobalState.store(static_cast<uint32_t>(state));
    workers[workerID]->notifyConsume();
}

void AriaCoordinator::syncWorkerState(uint32_t workerID, WorkerState state) {
    if (!workers.count(workerID)) {
        return ;
    }
    std::unique_lock<std::mutex> produceLock(workers[workerID]->mutex);
    while (workers[workerID]->workerState.load() != static_cast<uint32_t>(state)) {
        workers[workerID]->produce.wait(produceLock);
    }
}

void AriaCoordinator::printWorkerCondition() {
    uint32_t ready = 0;
    uint32_t assigned = 0;
    for (auto worker : workers){
        uint32_t state = worker.second->workerState;
        if (state == static_cast<uint32_t>(WorkerState::READY))
            ready++;
        if (state == static_cast<uint32_t>(WorkerState::ASSIGNED))
            assigned++;
    }
    LOG(INFO) << "Total:";
    LOG(INFO) << "    epoch number: " << this->epochChan;
    LOG(INFO) << "    worker number: "<< this->workers.size();
    LOG(INFO) << "    ready worker: "<< ready;
    LOG(INFO) << "    assigned worker: "<< assigned;
}

uint32_t AriaCoordinator::deployWorkload(std::shared_ptr<Workload> workload) {

    //从worker中找出处于Ready状态的，将任务分配给它
    for (auto worker : workers) {
        if (worker.second->workerState == static_cast<uint32_t>(WorkerState::READY)) {
            worker.second->workload = std::move(workload);
            worker.second->workerState = static_cast<uint32_t>(WorkerState::ASSIGNED);
            DLOG(INFO) << "deploying workload to a worker: " << worker.second->workerID << ", epoch:" << worker.second->workload->epoch;
            return worker.second->workerID;
        }
    }
    return createWorker(std::move(workload));
}

void AriaCoordinator::broadcastToAllWorker(bool onlyAssigned) {
    for (auto worker : workers) {
        std::unique_lock<std::mutex> produceLock(worker.second->mutex);
        if (onlyAssigned && worker.second->workerState.load() == static_cast<uint32_t>(WorkerState::READY))
            continue;
        worker.second->ariaGlobalState.store(static_cast<uint32_t>(globalState));
        worker.second->notifyConsume();
    }
}

uint32_t AriaCoordinator::destroyUnusedWorkers() {
    // destroy the worker after signaled exit
    uint32_t count = 0;
    auto worker = workers.begin();
    while (worker != workers.end()) {
        if (worker->second->workerState.load() == static_cast<uint32_t>(WorkerState::EXITED)) {
            worker->second->thread->join();
            delete worker->second;
            workers.erase(worker++);
            count++;
        } else {
            worker++;
        }
    }
    return count;
}

void AriaCoordinator::getWorkload(uint32_t &isCurrentEpochEmpty, std::shared_ptr<Workload> &workload) {
    //从transactionbuffer中，将交易打包成workload，
    std::unique_lock<std::mutex> transactionBufferLock(transactionBufferMutex);
    consume.wait(transactionBufferLock, [&] (){
        //如果transactionbuffer 为空 wait
        if (transactionBuffer.empty()) return false;

        //如果transactionbuffer中已经存在下一个epoch的交易， process
        if (transactionBuffer.back()->getEpoch() > epochChan) return true;
        //如果trsactionbuff中的交易数量太少 wait
        if (transactionBuffer.size() < COORDINATOR_MIN_TRANSACTION_PER_WORKLOAD) return false;
        //当前epoch的交易，并且数量足够
        return true;
    });

    Transaction* transaction = transactionBuffer.front();
    if (transaction->getEpoch() > epochChan.load()) {
        isCurrentEpochEmpty = true;
        return ;
    }

    workload = std::make_shared<Workload>();
    workload->epoch = transaction->getEpoch();
    isCurrentEpochEmpty = false;

    do {
        DCHECK(transaction->getEpoch() == epochChan.load());
        DCHECK(transaction->getEpoch() == workload->epoch);
        workload->transactionList.push_back(transaction);
        transactionBuffer.pop();
        transaction = transactionBuffer.front();
    } while (!transactionBuffer.empty() && //transaction 不为空
              transaction->getEpoch() == epochChan && //transactionbuffer里 还有当前epoch的交易
              workload->transactionList.size() < COORDINATOR_MIN_TRANSACTION_PER_WORKLOAD); //不超过workload的最大负载
    DLOG(INFO) << "created a workload, size: " << workload->transactionList.size() << ", epoch:" << workload->epoch;

    produce.notify_one();
}

uint32_t AriaCoordinator::addTransaction(Transaction *transaction) {
    //生产者函数
    std::unique_lock<std::mutex> lock(transactionBufferMutex);
    produce.wait(lock, [this] {
        return this->transactionBuffer.size() +  1 <= COORDINATOR_INPUT_MAX_BUFFER_SIZE;
    });
    transactionBuffer.push(transaction);

    consume.notify_one();
    return 0;
}
