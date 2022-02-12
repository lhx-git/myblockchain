
//
// Created by lhx on 1/20/22.
//

#ifndef MYBLOCKCHAIN_SPIN_LOCK_H
#define MYBLOCKCHAIN_SPIN_LOCK_H
#include <atomic>
class SpinLock {
public:
    SpinLock() = default;
    SpinLock(const SpinLock&) = delete;
    SpinLock& operator=(const SpinLock&) = delete;

    void lock() {
        while (_flag.test_and_set(std::memory_order_acquire));
    }

    void unlock() {
        _flag.clear(std::memory_order_release);
    }
private:
    std::atomic_flag _flag= ATOMIC_FLAG_INIT;
};
#endif //MYBLOCKCHAIN_SPIN_LOCK_H
