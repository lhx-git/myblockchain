//
// Created by lhx on 2/8/22.
//

#include "common/zmq/zmq_server.h"
#include "glog/logging.h"

ZMQServer::ZMQServer(const std::string &ip, const std::string &port, zmq::socket_type type) {
    context = new zmq::context_t;  //创建上下文
    socket = new zmq::socket_t(*context, type); //设置socket类型
    std::string address = "tcp://"+ ip +":" + port; //设置服务器的ip, 监听的port
    DLOG(INFO) << "listen address: " << address;
    socket->bind(address); //绑定port
}

std::optional<size_t> ZMQServer::getRequest(zmq::message_t& request) {
    return socket->recv(request);
}

std::optional<size_t> ZMQServer::sendReply(zmq::const_buffer &reply) {
    return socket->send(reply);
}

std::optional<size_t> ZMQServer::sendReply(const std::string &reply) {
    zmq::const_buffer buffer(reply.c_str(), reply.size());
    return socket->send(buffer);
}

std::optional<size_t> ZMQServer::getRequest(std::string &request) {
    zmq::message_t message;
    std::optional<size_t> size = socket->recv(message);
    request = message.to_string();
    return size;
}

ZMQServer::~ZMQServer() {
    delete socket;
    delete context;
}

std::optional<size_t> ZMQServer::getRequest() {
    std::string drop;
    return getRequest(drop);
}

std::optional<size_t> ZMQServer::sendReply() {
    std::string drop;
    return sendReply(drop);
}