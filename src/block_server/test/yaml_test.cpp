//
// Created by lhx on 2/7/22.
//

#include <yaml-cpp/yaml.h>
#include "glog/logging.h"
int main() {
    YAML::Node config = YAML::LoadFile("/home/lhx/Desktop/myblockchain/doc/config.yaml");
    bool init_crypto = config["init_crypto"].as<bool>();
    bool reset_epoch = config["reset_epoch"].as<bool>();
    DLOG(INFO) << "init_crypto" << " : " <<  init_crypto << std::endl;
    DLOG(INFO) << "reset_epoch" << " : " << reset_epoch << std::endl;

    YAML::Node serverIps = config["server_ips"];
    for (auto ips : serverIps) {
        DLOG(INFO) << "server_ips"  << " : " << ips.as<std::string>();
    }

    YAML::Node epochIps = config["epoch_ips"];
    for (auto ips : epochIps) {
        DLOG(INFO) << "epoch_ips"  << " : " << ips.as<std::string>();
    }

    YAML::Node server_data = config["server_data"];
    YAML::Node server1 = server_data["192.168.60.135"];
    YAML::Node server1_public_crt_file = server1["public_crt"];
    YAML::Node server1_private_crt_file = server1["private_crt"];
    YAML::Node server1_user_public_crt_file = server1["user_public_crt"];
    YAML::Node server1_user_private_crt_file = server1["user_private_crt"];

    DLOG(INFO) << "------------------" << "server1 data" << "------------------" << std::endl;
    DLOG(INFO) << server1_public_crt_file.as<std::string>() << std::endl;
    DLOG(INFO) << server1_private_crt_file.as<std::string>() << std::endl;
    DLOG(INFO) << server1_user_public_crt_file.as<std::string>() << std::endl;
    DLOG(INFO) << server1_user_private_crt_file.as<std::string>() << std::endl;


    return 0;
}