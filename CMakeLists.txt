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
include_directories(common)

SET(LIBRARY_DEPENDENCIES
#        aria-proto
#        libprotobuf.a
#        libzmq.a
#        libleveldb.a
#        libgflags.a
#        libyaml-cpp.so
        libglog.so
#        libpqxx.so
#        libcrypto.so
        )

ADD_SUBDIRECTORY(src/block_server)


