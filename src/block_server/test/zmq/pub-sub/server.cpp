//
// Created by lhx on 2/8/22.
//

#include "common/zmq/zmq_server.h"
#include "glog/logging.h"



int main() {
    std::unique_ptr<ZMQServer> server = std::make_unique<ZMQServer>("*", "9001" , zmq::socket_type::pub);

    int count = 1000;
    while (count--) {
        server->sendReply(std::to_string(count));
        DLOG(INFO) << "publish " << " : " << count << std::endl;
        sleep(1);
    }
    return 0;
}