cmake_minimum_required(VERSION 3.17)


SET(LIBRARY_DEPENDENCIES
        glog
        gflags
        )


FILE(GLOB SC_FILES
        "*.cpp"
        "worker/*.cpp"
        "coordinator/*.cpp"
        "transaction/*.cpp")
SET(BLOCK_SERVER_SOURCE
        server.cpp ../../include/block_server/transaction/mock/mock_transaction_manager.h transaction/mock_transaction_manager.cpp ../../include/block_server/transaction/mock/mock_transaction.h ../../include/block_server/message_format/workload.h)



ADD_EXECUTABLE(server  ${SC_FILES} ${BLOCK_SERVER_SOURCE})

TARGET_LINK_LIBRARIES(server ${LIBRARY_DEPENDENCIES} ${PROTOBUF_LIBRARY_DEBUG})