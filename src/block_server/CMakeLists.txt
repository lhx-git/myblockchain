CMAKE_MINIMUM_REQUIRED(VERSION 3.6 FATAL_ERROR)

FILE(GLOB BLOCK_SERVER_SRC
        coordinator/*.cpp
        worker/*.cpp
        worker/impl/*.cpp
        database/*.cpp
        database/impl/*.cpp
        database/orm/*.cpp
        database/orm/impl/*.cpp
        transaction/mock/*.cpp
        transaction/impl/*.cpp
        reserve_table/*.cpp
        reserve_table/mock/*.cpp
        comm/client_proxy/*.cpp
        comm/stub/*.cpp
        )

add_executable(mock_block_server mock_server.cpp ${BLOCK_SERVER_SRC} ${COMMON_SRC})
target_link_libraries(mock_block_server ${LIBRARY_DEPENDENCIES})


add_executable(block_server server.cpp ${BLOCK_SERVER_SRC} ${COMMON_SRC})
target_link_libraries(block_server ${LIBRARY_DEPENDENCIES})