//
// Created by peng on 4/9/21.
//

#include "common/yaml_config.h"
#include <mutex>
#include "glog/logging.h"

#define YAML_CONFIG_FILE "config.yaml"
#define BK_YAML_CONFIG_FILE "/tmp/config.yaml"

#define YAML_INIT_CRYPTO "init_crypto"
#define YAML_RESET_EPOCH "reset_epoch"

#define YAML_BLOCK_SERVER_IPS "server_ips"
#define YAML_LOCAL_BLOCK_SERVER_IP "local_server_ip"

#define YAML_BLOCK_SERVER_DATA "server_data"
#define YAML_BLOCK_SERVER_PUBLIC_PATH "public_crt"
#define YAML_BLOCK_SERVER_PRIVATE_PATH "private_crt"
#define YAML_USER_PUBLIC_PATH "user_public_crt"
#define YAML_USER_PRIVATE_PATH "user_private_crt"

#define YAML_EPOCH_SERVER_IPS "epoch_ips"

#define YAML_EPOCH_SERVER_DATA "epoch_data"
#define YAML_EPOCH_SERVER_PUBLIC_PATH "public_crt"
#define YAML_EPOCH_SERVER_PRIVATE_PATH "private_crt"

YAMLConfig::YAMLConfig(const std::string &fileName)
        :data(YAML::LoadFile(fileName)) {}

std::vector<std::string> YAMLConfig::getBlockServerIPs() const {
    std::vector<std::string> serverIPs;
    if(auto blockServers = data[YAML_BLOCK_SERVER_IPS]) {
        for(auto blockServer: blockServers) {
            serverIPs.push_back(blockServer.as<std::string>());
        }
    }
    return serverIPs;
}

size_t YAMLConfig::getBlockServerConst() const {
    if(auto blockServers = data[YAML_BLOCK_SERVER_IPS]) {
        return blockServers.size();
    }
    LOG(ERROR) << "Can not get server count!";
    return 0;
}

std::unique_ptr<YAMLConfig::ServerConfigStruct> YAMLConfig::getBlockServerConfig(const std::string &ip) const {
    if(auto serverData = data[YAML_BLOCK_SERVER_DATA][ip]) {
        auto serverConfig = std::make_unique<YAMLConfig::ServerConfigStruct>();
        serverConfig->ip = ip;
        serverConfig->pubFilePath = serverData[YAML_BLOCK_SERVER_PUBLIC_PATH].as<std::string>();
        serverConfig->priFilePath = serverData[YAML_BLOCK_SERVER_PRIVATE_PATH].as<std::string>();
        serverConfig->usrPubFilePath = serverData[YAML_USER_PUBLIC_PATH].as<std::string>();
        serverConfig->usrPriFilePath = serverData[YAML_USER_PRIVATE_PATH].as<std::string>();
        return serverConfig;
    }
    LOG(ERROR) << "Can not get server detail!";
    return nullptr;
}

YAMLConfig *YAMLConfig::getInstance() {
    static YAMLConfig* config = nullptr;
    static std::mutex mutex;
    if(config == nullptr) {  // performance
        std::unique_lock<std::mutex> lock(mutex);
        if(config == nullptr) {  // prevent for re-enter
            try {
                config = new YAMLConfig(YAML_CONFIG_FILE);
            }
            catch(const YAML::Exception& e) {
                LOG(ERROR) << YAML_CONFIG_FILE << " not exist, switch to bk file!";
                delete config;
                try {
                    config = new YAMLConfig(BK_YAML_CONFIG_FILE);
                }
                catch(const YAML::Exception& e) {
                    LOG(ERROR) << BK_YAML_CONFIG_FILE << " not exist!";
                    CHECK(false);
                }
            }
        }
    }
    return config;
}

bool YAMLConfig::initCrypto() const {
    return data[YAML_INIT_CRYPTO].as<bool>();
}

std::string YAMLConfig::getLocalBlockServerIP() const {
    return data[YAML_LOCAL_BLOCK_SERVER_IP].as<std::string>();
}

std::vector<std::string> YAMLConfig::getEpochServerIPs() const {
    std::vector<std::string> serverIPs;
    if(auto blockServers = data[YAML_EPOCH_SERVER_IPS]) {
        for(auto blockServer: blockServers) {
            serverIPs.push_back(blockServer.as<std::string>());
        }
    }
    return serverIPs;
}

std::unique_ptr<YAMLConfig::EpochConfigStruct> YAMLConfig::getEpochServerConfig() const {
    if(auto serverData = data[YAML_EPOCH_SERVER_DATA]) {
        auto serverConfig = std::make_unique<YAMLConfig::EpochConfigStruct>();
        serverConfig->pubFilePath = serverData[YAML_EPOCH_SERVER_PUBLIC_PATH].as<std::string>();
        serverConfig->priFilePath = serverData[YAML_EPOCH_SERVER_PRIVATE_PATH].as<std::string>();
        return serverConfig;
    }
    LOG(ERROR) << "Can not get epoch server detail!";
    return nullptr;
}

bool YAMLConfig::resetEpoch() const {
    return data[YAML_RESET_EPOCH].as<bool>();
}