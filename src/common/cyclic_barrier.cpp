#include <chrono>
#include <utility>
#include "common/cyclic_barrier.h"

cbar::CyclicBarrier::CyclicBarrier(uint32_t _parties, std::function<void()>  _callback)
    : callback(std::move(_callback)), current_waits(0), parties(_parties) { }

void cbar::CyclicBarrier::await(uint64_t nano_secs) {
    std::unique_lock<std::mutex> lck(lock);
    if (current_waits < parties) {
        current_waits++;
    }
    if (current_waits >= parties) {
        std::lock_guard<std::mutex> rst(reset_lock);
        if (callback != nullptr) {
            callback();
        }
        current_waits = 0;
        lck.unlock();
        cv.notify_all();
        return;
    } else {
        if (nano_secs > 0) {
            cv.wait_for(lck, std::chrono::nanoseconds(nano_secs));
        } else {
            cv.wait(lck);
        }
    }
    lck.unlock();
}

void cbar::CyclicBarrier::reset() {
    lock.lock();
    std::lock_guard<std::mutex> rst(reset_lock);
    if (callback != nullptr) {
        callback();
    }
    current_waits = 0;
    lock.unlock();
    cv.notify_all();
}

uint32_t cbar::CyclicBarrier::get_barrier_size() const {
    return parties;
}

uint32_t cbar::CyclicBarrier::get_current_waiting() const {
    return current_waits;
}
