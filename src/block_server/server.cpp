//
// Created by lhx on 2/11/22.
//

#include "blockserver/transaction/mock/mock_transaction.h"
#include "blockserver/coordinator/AriaCoordinator.h"
#include "blockserver/transaction/impl/transaction_manager_impl.h"
#include "blockserver/transaction/transaction_wrapper.h"
#include "thread"
#include "glog/logging.h"
#include "common/yaml_config.h"
#include "common/crypto_sign.h"

int main() {
    std::string password;
    auto* config = YAMLConfig::getInstance();
    if(config->initCrypto()) {
        // init crypto
        //1. generate block servers and user crypto.
        for(const auto& ip: config->getBlockServerIPs()) {
            auto serverConfig = config->getBlockServerConfig(ip);
            CryptoSign::generateKeyFiles(serverConfig->pubFilePath.data(), serverConfig->priFilePath.data(), reinterpret_cast<const unsigned char *>(password.data()), password.size());
            CryptoSign::generateKeyFiles(serverConfig->usrPubFilePath.data(), serverConfig->usrPriFilePath.data(), reinterpret_cast<const unsigned char *>(password.data()), password.size());
        }
        //2. generate epoch server crypto.
        auto serverConfig = config->getEpochServerConfig();
        CryptoSign::generateKeyFiles(serverConfig->pubFilePath.data(), serverConfig->priFilePath.data(), reinterpret_cast<const unsigned char *>(password.data()), password.size());
    }

    std::atomic<uint32_t> ep;
    ep.store(1);

    auto coordinator = new AriaCoordinator(ep);
    std::thread coordinatorThread(&AriaCoordinator::run, coordinator);
    auto transactionManager = new TransactionManagerImpl();
    coordinator->epochTransactionFinishSignal = [&] (uint32_t epoch) {
        transactionManager->lastEpochFinishSignal = epoch;
    };
    transactionManager->sendTransactionHandle = [&] (TransactionWrapper* transactionWrapper) {
        coordinator->addTransaction(transactionWrapper);
    };
    std::thread transactionManagerThread(&TransactionManager::run, transactionManager);
    coordinatorThread.join();
    transactionManagerThread.join();

    return 0;
}