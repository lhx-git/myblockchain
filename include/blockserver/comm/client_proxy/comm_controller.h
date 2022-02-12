
//
// Created by lhx on 2/9/22.
//

#ifndef MYBLOCKCHAIN_COMM_CONTROLLER_H
#define MYBLOCKCHAIN_COMM_CONTROLLER_H
#include "common/zmq/zmq_server.h"
#include "blockserver/comm/comm.h"
#include "common/concurrent_queue/blocking_concurrent_queue.hpp"
#include "common/hash_map.h"
#include "common/cyclic_barrier.h"
#include <queue>
#include <map>

class UserCollector;
class Transaction;
class ZMQClient;
class CryptoSign;
class SHA256Helper;

namespace comm {
    class TransactionsWithProof;
    class EpochResponse;
}

class CommController: public ZMQServer, public Comm {
public:
    CommController();
    ~CommController() override;

    void receiveFromLocalServer();

    void receiveFromOtherServer(ZMQClient* client);

    TransactionWrapper* getTransaction(uint32_t epoch, uint32_t maxSize, uint32_t minSize) override;
    void clearTransactionForEpoch(uint32_t epoch) override;

    void wait();

protected:
    void transferTransaction();

    bool verifyBatchHash(const comm::TransactionsWithProof* transactionSet,const comm::EpochResponse& response);

private:
    //待处理pendingTransactions 队列存储经过epoch服务器打epoch号的交易,但是并未经过验证完整性及 epohc签名正确性的交易
    //无序braodcastTransactions 存储经过验证,并计算好tid的交易.   其他blk server的交易也在这个容器中, epoch号相同的交易相邻存放
    //排序processedTransaction 利用优先级队列的特性来有序的取出交易, 由transactionManager负责从这个队列中添加(transferTransaction)和取出元素(getTransaction).
    uint32_t epoch; //已经收集全的 最新的epoch号
    std::atomic<bool> finishSignal;
    UserCollector* userCollector;
    //用于接收用户发送来的消息
    moodycamel::BlockingConcurrentQueue<Transaction*> pendingTransactions;
    moodycamel::BlockingConcurrentQueue<std::string> pendingTransactionsProof;
    //一个epoch所有交易
    moodycamel::BlockingConcurrentQueue<Transaction*> broadcastTransactions;

    // 用于同步多个 blockserver
    cbar::CyclicBarrier barrier;
    HashMap<997, uint32_t, size_t> epochSizeMap;

    std::thread* userCollectorThread = nullptr;
    std::thread* localReceiverThread = nullptr;
    std::vector<std::thread*> remoteReceiverThreads;

    //排序
    std::priority_queue<Transaction*, std::vector<Transaction*>,
            std::function<bool(Transaction*, Transaction*)>> processedTransaction;
    //epoch : 对应epoch的transactions
    std::map<uint32_t ,std::vector<Transaction*>> transactionMap;

    CryptoSign* epochServerSignHelper;
    SHA256Helper* sha256Helper;
};
#endif //MYBLOCKCHAIN_COMM_CONTROLLER_H
