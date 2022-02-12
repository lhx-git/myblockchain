//
// Created by lhx on 2/12/22.
//
#include "common/zmq/zmq_server.h"
#include "common/yaml_config.h"
#include "common/complie_config.h"
#include "common/crypto_sign.h"
#include "comm.pb.h"
#include "glog/logging.h"

int main() {
    std::map<uint32_t ,size_t> epochSizeMap;
    ZMQServer server("0.0.0.0", "6002");
    auto epochServerConfig = YAMLConfig::getInstance()->getEpochServerConfig();
    CryptoSign epochServerSignHelper(epochServerConfig->pubFilePath, epochServerConfig->priFilePath);

    uint32_t epochNum = 1;
    time_t epochStartTime = time(nullptr);
    size_t totalSize = 0;
    while(true) {
        // server receive from user-proxy client, block
        std::string requestRaw;
        server.getRequest(requestRaw);

        // first, we check if current epoch size == 0, to reset timer
        if(epochSizeMap[epochNum] == 0) {
            epochStartTime = time(nullptr);
        }
        // second, process request and update env value
        comm::EpochResponse response;
        comm::EpochRequest* request = response.mutable_request();
        request->ParseFromString(requestRaw);
        size_t currentRequestSize = request->request_batch_size();

        totalSize += currentRequestSize;
        epochSizeMap[epochNum] += currentRequestSize;
        // print transaction info
        DLOG(INFO) << "-----------------------";
        DLOG(INFO) << "transaction size: " << currentRequestSize;
        DLOG(INFO) << "processed transactions size: " << totalSize;
        DLOG(INFO) << "current transactions size: " << epochSizeMap[epochNum];
        DLOG(INFO) << "current epoch: " << epochNum;

        // send response
        response.set_epoch(epochNum);
        response.set_nonce(totalSize);
        // calculate signature
        std::string signature;
        epochServerSignHelper.rsaEncrypt(std::to_string(response.epoch()) +
                                         request->batch_hash() +
                                         std::to_string(response.nonce()), &signature);
        // append signature
        response.set_signature(signature);
        server.sendReply(response.SerializeAsString());
        // update epoch number
        if(difftime(time(nullptr), epochStartTime) >= static_cast<double>(TIME_GAP)) {
            LOG(INFO) << "Epoch: "<< epochNum << ", transaction size: " << epochSizeMap[epochNum];
            // we do not reset epoch start time in here
            epochNum += 1;
        }
    }
}
