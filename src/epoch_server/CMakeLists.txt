CMAKE_MINIMUM_REQUIRED(VERSION 3.6 FATAL_ERROR)

add_executable(epoch_server epoch_server.cpp ${BLOCK_SERVER_SRC} ${COMMON_SRC})
target_link_libraries(epoch_server ${LIBRARY_DEPENDENCIES})