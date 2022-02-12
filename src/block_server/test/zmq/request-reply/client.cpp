//
// Created by lhx on 2/8/22.
//

#include "common/zmq/zmq_client.h"
#include "glog/logging.h"

int main() {
    std::unique_ptr<ZMQClient> client = std::make_unique<ZMQClient>("127.0.0.1", "9001");
    int count = 1000;
    while (count--) {
        client->sendRequest(std::to_string(count));
        std::string rep;
        client->getReply(rep);
        DLOG(INFO) << "recv server rep" << " : " << rep << std::endl;
    }
    return 0;
}