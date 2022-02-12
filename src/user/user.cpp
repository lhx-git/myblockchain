//
// Created by lhx on 2/12/22.
//

#include <string>
#include "common/zmq/zmq_client.h"
#include <thread>
#include "common/crypto_sign.h"
#include "common/yaml_config.h"
#include "common/random.h"
#include "comm.pb.h"
#include "tpc-c.pb.h"
#include "transaction.pb.h"
#include "glog/logging.h"

void run(std::shared_ptr<ZMQClient> client, bool sleepFlag) {
    comm::UserRequest request;
    TransactionPayload payload;

    //payload.set_header("mixed");
    payload.set_header("crdt_gcounter");
    //todo lhx modify here
    CRDT_GCOUNTER_PAYLOAD crdtGcounterPayload;
    YCSB_PAYLOAD ycsbPayload;
    auto blockServerConfig = YAMLConfig::getInstance()->getBlockServerConfig(YAMLConfig::getInstance()->getLocalBlockServerIP());
    CryptoSign userSignHelper(blockServerConfig->usrPubFilePath, blockServerConfig->usrPriFilePath);
    Random randomHelper;
    uint64_t trCount = 0;
    while (true) {
        crdtGcounterPayload.Clear();
        crdtGcounterPayload.set_key("lhx");
        crdtGcounterPayload.set_value(1);
        /*ycsbPayload.Clear();
        int readSize = 2;
        int writeSize = 4 - readSize;
        for (int i = 0; i < readSize; i++) {
            *ycsbPayload.add_reads() = std::to_string(randomHelper.uniform_dist(0, 200000));
        }

        for (int i = 0; i < writeSize; i++) {
            *ycsbPayload.add_update() = std::to_string(randomHelper.uniform_dist(0, 200000));
        }*/

        //set message
        payload.set_payload(crdtGcounterPayload.SerializeAsString());
        payload.set_nonce(time(nullptr) + (trCount << 32));
        trCount++;
        std::string payloadRaw = payload.SerializeAsString();
        // calculate digest
        std::string digest;
        userSignHelper.rsaEncrypt(payloadRaw, &digest);

        request.set_payload(payloadRaw);
        request.set_digest(digest);
        client->sendRequest(request.SerializeAsString());
        client->getReply();

        // if light weight, sleep
        if (sleepFlag) {
            usleep(100000);
        }
    }
}

int main(int argc, char *argv[]) {
    bool sleepFlag = false;
    uint32_t thread_num = 1;
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-t") == 0) {
            thread_num = std::stoi(argv[i + 1]);
        }
        if (strcmp(argv[i], "-s") == 0) {
            sleepFlag = true;
        }
    }

    //启动thread_num个线程向blockServer发送transaction
    std::vector<std::unique_ptr<std::thread>> threads;
    for (int i = 0; i < thread_num; i++) {
        threads.push_back(std::make_unique<std::thread>(&run, std::make_shared<ZMQClient>("127.0.0.1", "5001"), sleepFlag));
    }

    for (int i = 0; i < thread_num; i++) {
        threads[i]->join();
    }

    return 0;
}