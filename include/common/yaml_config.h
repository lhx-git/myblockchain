//
// Created by peng on 2/8/22.
//

#ifndef MYBLOCKCHAIN_YAML_CONFIG_H
#define MYBLOCKCHAIN_YAML_CONFIG_H
#include <yaml-cpp/yaml.h>
#include <memory>

class YAMLConfig {
public:
    struct ServerConfigStruct {
        std::string ip;
        std::string priFilePath;
        std::string pubFilePath;
        std::string usrPriFilePath;
        std::string usrPubFilePath;
    };

    struct EpochConfigStruct {
        std::string priFilePath;
        std::string pubFilePath;
    };

    static YAMLConfig* getInstance();
    ~YAMLConfig() = default;
    bool initCrypto() const;
    bool resetEpoch() const;
    size_t getBlockServerConst() const;
    std::vector<std::string> getBlockServerIPs() const;
    std::vector<std::string> getEpochServerIPs() const;
    std::string getLocalBlockServerIP() const;
    //get某个ip的server配置
    std::unique_ptr<ServerConfigStruct> getBlockServerConfig(const std::string& ip) const;
    //get epoch的配置
    std::unique_ptr<EpochConfigStruct> getEpochServerConfig() const;

protected:
    explicit YAMLConfig(const std::string &fileName);

private:
    const YAML::Node data;
};
#endif //MYBLOCKCHAIN_YAML_CONFIG_H
