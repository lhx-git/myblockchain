//
// Created by lhx on 2/8/22.
//

#ifndef MYBLOCKCHAIN_CLIENT_H
#define MYBLOCKCHAIN_CLIENT_H

//https://www.cnblogs.com/silence-cho/p/12657234.html  zmq三种模式示例

//sub-pub模式
//https://blog.csdn.net/guotianqing/article/details/98043497

#include <string>
#include <optional>
#include <zmq.hpp>

class ZMQClient {
public:
    ZMQClient(const std::string& ip, const std::string& port, zmq::socket_type type = zmq::socket_type::req);
    virtual ~ZMQClient();

    std::optional<size_t> sendRequest();
    std::optional<size_t> sendRequest(zmq::const_buffer& request);
    std::optional<size_t> sendRequest(const std::string& request);
    std::optional<size_t> getReply();
    std::optional<size_t> getReply(zmq::message_t& reply);
    std::optional<size_t> getReply(std::string& reply);

private:
    zmq::context_t* context;
    zmq::socket_t* socket;
};

#endif //MYBLOCKCHAIN_CLIENT_H
