//
// Created by lhx on 2/8/22.
//

#ifndef MYBLOCKCHAIN_SERVER_H
#define MYBLOCKCHAIN_SERVER_H

#include <string>
#include <optional>
#include <zmq.hpp>

class ZMQServer {
public:
    ZMQServer(const std::string& ip, const std::string& port, zmq::socket_type type = zmq::socket_type::rep);
    virtual ~ZMQServer();

    std::optional<size_t> getRequest();
    std::optional<size_t> getRequest(zmq::message_t& request);
    std::optional<size_t> getRequest(std::string& request);
    std::optional<size_t> sendReply();
    std::optional<size_t> sendReply(zmq::const_buffer& reply);
    std::optional<size_t> sendReply(const std::string& reply);

private:
    zmq::context_t* context;
    zmq::socket_t* socket;
};

#endif //MYBLOCKCHAIN_SERVER_H
