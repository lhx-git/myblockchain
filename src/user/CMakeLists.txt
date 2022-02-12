CMAKE_MINIMUM_REQUIRED(VERSION 3.6 FATAL_ERROR)

add_executable(user user.cpp ${BLOCK_SERVER_SRC} ${COMMON_SRC})
target_link_libraries(user ${LIBRARY_DEPENDENCIES})