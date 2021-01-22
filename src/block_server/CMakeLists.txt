cmake_minimum_required(VERSION 3.17)

FILE(GLOB SC_FILES
        "*.cpp"
        "worker/*.cpp"
        "coordinator/*.cpp")
SET(BLOCK_SERVER_SOURCE
        server.cpp ../../include/block_server/coordinator/aria_coordinator.h coordinator/aria_coordinator.cpp ../../include/block_server/coordinator/coordinator.h ../../include/block_server/message_format/config.h)
ADD_EXECUTABLE(server  ${SC_FILES} ${BLOCK_SERVER_SOURCE})