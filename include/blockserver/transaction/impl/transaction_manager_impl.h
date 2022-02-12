//
// Created by lhx on 2/10/22.
//

#ifndef MYBLOCKCHAIN_TRANSACTION_MANAGER_IMPL_H
#define MYBLOCKCHAIN_TRANSACTION_MANAGER_IMPL_H

#include "blockserver/transaction/transaction_manager.h"
#include "blockserver/comm/comm.h"
#include "blockserver/database/block_generator.h"
#include <memory>
//1.从comm模块取出 transactionWrapper. 发送给Coordinator
//2.统计上一个epoch处理完成的交易, 并且成生区块
class TransactionManagerImpl : public TransactionManager {
public:
    TransactionManagerImpl();
    ~TransactionManagerImpl();
    [[noreturn]] void run() override;
private:
    std::unique_ptr<TransactionWrapper> currentEpochTrWrapper;
    std::unique_ptr<TransactionWrapper>  nextEpochTrWrapper;
    BlockGenerator* blockGenerator{};
    uint32_t epoch = 0; // epoch start from 0
    Comm* commModule{};
};
#endif //MYBLOCKCHAIN_TRANSACTION_MANAGER_IMPL_H
