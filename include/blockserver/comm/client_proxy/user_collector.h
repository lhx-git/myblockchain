//
// Created by lhx on 2/9/22.
//

#ifndef MYBLOCKCHAIN_USER_COLLECTOR_H
#define MYBLOCKCHAIN_USER_COLLECTOR_H

#include "common/zmq/zmq_server.h"
#include "common/zmq/zmq_client.h"
#include "common/concurrent_queue/blocking_concurrent_queue.hpp"


class Transaction;
//clientproxy任务

/*usercollector
//1.收集user发来的request  使用并发队列, thread1 collect request
//2.将交易发送给epoch服务器
 */

//3.当接收完来自user的一个epoch的交易后,广播给其他的blockserver
//4.接收其他blockserver发来的交易
//

class UserCollector : public ZMQServer {
public:
    UserCollector();
    ~UserCollector();
    void collectRequest();
    void run();

    //与CommController对象通信,回调函数一般是由其他对象进行定义的
    std::function<void(const std::vector<Transaction*>& transaction, const std::string& proof)> newTransactionHandle;
    std::function<void(uint32_t, size_t)> lastEpochSizeHandle;

private:
    std::atomic<bool> finishSignal;
    std::thread* collector;
    moodycamel::BlockingConcurrentQueue<std::string*> requestBuffer;
    ZMQClient* epochServerClient;
};

#endif //MYBLOCKCHAIN_USER_COLLECTOR_H
