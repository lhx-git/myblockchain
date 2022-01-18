CMAKE_MINIMUM_REQUIRED(VERSION 3.6 FATAL_ERROR)

FILE(GLOB
        "coordinator/*.cpp"
        "worker/*.cpp")

SET(SRC coordinator/AriaCoordinator.cpp
        worker/AriaWorker.cpp)
#add_executable(block_server mock_server.cpp ${GLOB})
add_executable(block_server mock_server.cpp ${SRC})

target_link_libraries(block_server ${LIBRARY_DEPENDENCIES})