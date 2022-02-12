//
// Created by lhx on 1/17/22.
//

#ifndef MYBLOCKCHAIN_COMPLIE_CONFIG_H
#define MYBLOCKCHAIN_COMPLIE_CONFIG_H

//epoch号更新的时间间隔
#define TIME_GAP 1

//下面5个宏 成比例改变
#define SCALE 100

// coordinator 最大的缓存的交易大小
// 当epoch增加或者buffer满了,coordinator把交易分配给worker
#define COORDINATOR_INPUT_MAX_BUFFER_SIZE 50*SCALE
// 分配给worker的最大的trs, this must <= COORDINATOR_INPUT_MAX_BUFFER_SIZE
#define COORDINATOR_MAX_TRANSACTION_PER_WORKLOAD 10*SCALE
// 分配给worker的最小的trs, this must <= COORDINATOR_MAX_TRANSACTION_PER_WORKLOAD
#define COORDINATOR_MIN_TRANSACTION_PER_WORKLOAD 5*SCALE

// tr manager从comm接口获取的最大的batch size
#define TR_MGR_GET_BATCH_MAX_SIZE 5*SCALE
// tr manager从comm接口获取的最小的batch size
#define TR_MGR_GET_BATCH_MIN_SIZE 5*SCALE


#define ARIA_SYS_TABLE "aria_sys_table"

//#define COMM_MODE_LOCAL_ONLY


#ifndef COMM_MODE_LOCAL_ONLY
#define COMM_MODULE CommController
#else
#define COMM_MODULE CommStub
#endif

// dequeue default time: 0.2s
#define DEQUEUE_TIMEOUT_US 200000
// dequeue default size: 100
#define EPOCH_SIZE_APPROX 500

// define: use level db
// undef: use hash_map
//#define USING_KV_DB
#define USING_LEVEL_DB



// define: use reserve table implemented by mvcc reverse table
// undef: use hash map reserve table instead
//#define USING_MVCC_RESERVE_TABLE

// define: use spin lock in reserve table (maybe speed up or not)
// undef: use std::mutex instead
//#define USING_SPIN_LOCK


#ifndef  USING_MVCC_RESERVE_TABLE
#define GET_RESERVE_TABLE_TYPE getHashMapReserveTable
#else
#define GET_RESERVE_TABLE_TYPE getMVCCReserveTable
#endif

#ifdef USING_SPIN_LOCK
#define HASH_MAP_LOCK_TYPE SpinLock
#else
#define HASH_MAP_LOCK_TYPE std::mutex
#endif

#ifdef USING_LEVEL_DB
#define DB_STORAGE_TYPE LevelDBStorage
#else
#define DB_STORAGE_TYPE HashMapStorage
#endif

#endif //MYBLOCKCHAIN_COMPLIE_CONFIG_H
