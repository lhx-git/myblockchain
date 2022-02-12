//
// Created by lhx on 1/26/22.
//

#include "blockserver/worker/impl/transaction_executor_impl.h"
#include "blockserver/worker/impl/orm_test_chaincode.h"
#include "blockserver/transaction/transaction.h"
#include "blockserver/reserve_table/reserve_table.h"
#include "blockserver/reserve_table/transaction_dependency.h"
#include "transaction.pb.h"
#include "blockserver/message_fomat/transaction_result.h"
#include "blockserver/database/database.h"
#include "glog/logging.h"

void TransactionExecutorImpl::executeList(std::vector<Transaction *> transactionList) {
    for (auto transaction : transactionList) {
        //构造参数
        std::vector<std::string> args = { transaction->getTransactionPayload()->payload() };
        //调用智能合约
        ORMTestChaincode ormTestChaincode(transaction);
        if (!ormTestChaincode.InvokeChaincode(transaction->getTransactionPayload()->header(),args)) {
            //如果交易内部错误，直接废弃交易
            transaction->setTransactionResult(TransactionResult::ABORT_NO_RETRY);
            continue;
        }
        //构造保留表
        this->reserveTable = ReserveTableProvider::getHandle()->getReserveTable(transaction->getEpoch());
        DCHECK(reserveTable != nullptr);
        this->reserveTable->reserveRWset(transaction->getRWSet(), transaction->getTransactionID());
    }
}

void TransactionExecutorImpl::commitList(std::vector<Transaction *> transactionList) {
    auto dbInstance = VersionedDB::getDBInstance();
    for(auto transaction: transactionList) {
        if (transaction->getTransactionResult() == TransactionResult::ABORT_NO_RETRY) {
            // 1. transaction internal error, abort it without dealing with reserve table
            dbInstance->abortUpdate(transaction->getTransactionID());
            continue;
        }
        // 2. analyse dependency
        auto&& trDependency = reserveTable->dependencyAnalysis(transaction->getRWSet(), transaction->getTransactionID());
        if (trDependency.waw) { // waw, abort the transaction.
            transaction->setTransactionResult(TransactionResult::ABORT);
            continue;
        } else if (!trDependency.war || !trDependency.raw) {    //  war / raw / no dependency, commit it.
            if(dbInstance->commitUpdate(transaction->getTransactionID())) {
                transaction->setTransactionResult(TransactionResult::COMMIT);
            } else {
                LOG(ERROR) << "tr abort with rw set error: " << transaction->getTransactionID();
                transaction->setTransactionResult(TransactionResult::ABORT_NO_RETRY);
            }
            continue;
        } else {    // war and raw, abort the transaction.
            dbInstance->abortUpdate(transaction->getTransactionID());
            transaction->setTransactionResult(TransactionResult::ABORT);
        }
    }
    //crdt写法
//    //提交交易
//    VersionedDB::getDBInstance()->commitUpdate(transactionList);
//    //设置交易结果
//    for (auto tr : transactionList) {
//        tr->setTransactionResult(TransactionResult::COMMIT);
//    }
}