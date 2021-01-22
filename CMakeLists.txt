cmake_minimum_required(VERSION 3.17)
project(myblockchain)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")


set(CMAKE_CXX_STANDARD 14)
INCLUDE_DIRECTORIES(include)
ADD_SUBDIRECTORY(src)
