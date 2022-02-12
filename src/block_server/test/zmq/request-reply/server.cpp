//
// Created by lhx on 2/8/22.
//

#include "common/zmq/zmq_server.h"
#include "glog/logging.h"

int main() {
    std::unique_ptr<ZMQServer> server = std::make_unique<ZMQServer>("127.0.0.1", "9001");
    while (true) {
        std::string request;
        server->getRequest(request);
        DLOG(INFO) << "recv client request" << " : " << request << std::endl;
        std::string rep = "ack";
        server->sendReply(rep);
    }
    return 0;
}