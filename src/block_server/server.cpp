#include <iostream>
#include "block_server/worker/aria_worker.h"
#include "block_server/coordinator/aria_coordinator.h"
#include "block_server/message_format/config.h"
#include "common/hash_map.h"
#include <thread>
#include <chrono>
#include <map>


int main() {


    Config config;
    config.worker_num = 8;
    auto ariaCoordinator = std::make_shared<AriaCoordinator>(config);
    std::thread t1(&AriaCoordinator::run,ariaCoordinator.get());
    t1.join();

}


