CMAKE_MINIMUM_REQUIRED(VERSION 3.6 FATAL_ERROR)
project(myblockchain)

IF(NOT DEFINED BUILD_TEST)
    SET(BUILD_TEST OFF)
ENDIF()

IF(${BUILD_TEST})
    ENABLE_TESTING()
ENDIF()


SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED on)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99") #设置C的编译选项

IF(${CMAKE_CXX_COMPILER} STREQUAL "/usr/bin/clang++")
    SET(CMAKE_CXX_FLAGS_COMMON
            "-std=c++17 \
      -stdlib=libc++ -pthread")
ENDIF()

IF(${CMAKE_CXX_COMPILER} STREQUAL "/usr/bin/g++")
    SET(CMAKE_CXX_FLAGS_COMMON
            "-std=c++17 -pthread")
ENDIF()

SET(CMAKE_CXX_FLAGS_DEBUG
        "${CMAKE_CXX_FLAGS_DEBUG} \
    ${CMAKE_CXX_FLAGS_COMMON} \
    -g -O0 -fprofile-arcs -ftest-coverage")

SET(CMAKE_CXX_FLAGS_RELEASE
        "${CMAKE_CXX_FLAGS_RELEASE} \
    ${CMAKE_CXX_FLAGS_COMMON} \
    -O3")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(include)

INCLUDE(FindProtobuf)
FIND_PACKAGE(Protobuf REQUIRED "3.14.0")

if (Protobuf_FOUND)  # 需要特别注意Protobuf_FOUND的大小写
    INCLUDE_DIRECTORIES(${PROTOBUF_INCLUDE_DIR})
else (Protobuf_FOUND)
    MESSAGE(FATAL_ERROR "PROTOBUF NOT FOUNED")
endif (Protobuf_FOUND)

link_directories(${PROTOBUF_LIBRARIES})

INCLUDE_DIRECTORIES(${PROTOBUF_INCLUDE_DIR})
PROTOBUF_GENERATE_CPP(PROTO_SRC PROTO_HEADER
        ./include/proto/transaction.proto
        ./include/proto/chaincode.proto
        ./include/proto/common.proto
        ./include/proto/kv_rwset.proto
        ./include/proto/comm.proto
        ./include/proto/tpc-c.proto
        ./include/proto/db_test_chaincode.proto
        ./include/proto/block.proto
        )

ADD_LIBRARY(aria-proto ${PROTO_HEADER} ${PROTO_SRC})

FIND_PACKAGE(cppzmq)

FIND_PACKAGE(yaml-cpp REQUIRED)
INCLUDE_DIRECTORIES(${YAML_INCLUDE_DIRS})

FILE(GLOB COMMON_SRC
        "src/common/*.cpp"
        "src/common/zmq/*.cpp"
        "src/common/*.c"
        "src/common/merkle_tree/*.c")

SET(LIBRARY_DEPENDENCIES
        aria-proto
        libprotobuf.a
        libzmq.a
        libleveldb.a
#        libgflags.a
        libyaml-cpp.a
        libglog.so
#        libpqxx.so
        libcrypto.so
        )

ADD_SUBDIRECTORY(src/block_server)
ADD_SUBDIRECTORY(src/epoch_server)
ADD_SUBDIRECTORY(src/user)
ADD_SUBDIRECTORY(src/block_server/test)


