CMAKE_MINIMUM_REQUIRED(VERSION 3.6 FATAL_ERROR)

#crypto_sign测试
#add_executable(test test/crypto_sign_test.cpp ${COMMON_SRC})
#target_link_libraries(test ${LIBRARY_DEPENDENCIES})

#sha256_helper测试
#add_executable(test test/sha256_helper.cpp ${COMMON_SRC})
#target_link_libraries(test ${LIBRARY_DEPENDENCIES})

#yaml测试
#add_executable(test test/yaml_test.cpp ${COMMON_SRC})
#target_link_libraries(test ${LIBRARY_DEPENDENCIES})

#zmq测试 req-rep
#add_executable(test_client test/zmq/request-reply/client.cpp ${COMMON_SRC})
#target_link_libraries(test_client ${LIBRARY_DEPENDENCIES})

#add_executable(test_server test/zmq/request-reply/server.cpp ${COMMON_SRC})
#target_link_libraries(test_server ${LIBRARY_DEPENDENCIES})

#zmq测试 pub-sub
#add_executable(test_client1 test/zmq/pub-sub/client1.cpp ${COMMON_SRC})
#target_link_libraries(test_client1 ${LIBRARY_DEPENDENCIES})
#
#add_executable(test_client2 test/zmq/pub-sub/client2.cpp ${COMMON_SRC})
#target_link_libraries(test_client2 ${LIBRARY_DEPENDENCIES})
#
#add_executable(test_server test/zmq/pub-sub/server.cpp ${COMMON_SRC})
#target_link_libraries(test_server ${LIBRARY_DEPENDENCIES})

#concurrent_queue测试
add_executable(test_concurrent_queue concurrent_queue.cpp ${COMMON_SRC})
target_link_libraries(test_concurrent_queue ${LIBRARY_DEPENDENCIES})

#barrier_test
#add_executable(test_barrier barrier_test.cpp ${COMMON_SRC})
#target_link_libraries(test_barrier ${LIBRARY_DEPENDENCIES})
