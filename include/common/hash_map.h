//
// Created by lhx on 1/19/22.
//

#ifndef MYBLOCKCHAIN_HASHMAP_H
#define MYBLOCKCHAIN_HASHMAP_H
#include <unordered_map>
#include <mutex>
#include "spin_lock.h"
#include "complie_config.h"
/*支持并发的哈系表
* 此HashMap能够进行并发处理的方法， 内部包含多个哈系表， 每个哈系表对应一个; 一个哈系表同一时刻只能由一个线程获取，多个线程同时操作不同的哈系表，从而来支持并发。
*/
template <std::size_t N, class KeyType, class ValueType> class HashMap {

private:
    using HashMapType = std::unordered_map<KeyType, ValueType>;
    using HasherType = typename HashMapType::hasher;

public:
    bool remove(const KeyType &key) {
        return apply(
                [&key](HashMapType &map) {
                    auto it = map.find(key);
                    if (it == map.end()) {
                        return false;
                    } else {
                        map.erase(it);
                        return true;
                    }
                },
                bucket_number(key));
    }

    bool contains(const KeyType &key) {
        return apply(
                [&key](const HashMapType &map) { return map.find(key) != map.end(); },
                bucket_number(key));
    }

    bool insert(const KeyType &key, const ValueType &value) {
        return apply(
                [&key, &value](HashMapType &map) {
                    if (map.find(key) != map.end()) {
                        return false;
                    }
                    map[key] = value;
                    return true;
                },
                bucket_number(key));
    }

    ValueType &operator[](const KeyType &key) {
        return apply_ref(
                [&key](HashMapType &map) -> ValueType & { return map[key]; },
                bucket_number(key));
    }

    std::size_t size() {
        return fold(0, [](std::size_t totalSize, const HashMapType &map) {
            return totalSize + map.size();
        });
    }

    void clear() {
        map([](HashMapType &map) { map.clear(); });
    }

private:

    template <class ApplyFunc>
    auto &apply_ref(ApplyFunc applyFunc, std::size_t i) {
        // DCHECK(i < N) << "index " << i << " is greater than " << N;
        locks[i].lock();
        auto &result = applyFunc(maps[i]);
        locks[i].unlock();
        return result;
    }

    //apply实现了很灵活的设计，applyFunc是可变的，可以将lambda表达式赋值给他，
    //用insert（key， value）举例
    //applyFunc逻辑是插入操作的逻辑，i决定插入到那个map中。
    template <class ApplyFunc> auto apply(ApplyFunc applyFunc, std::size_t i) {
        // DCHECK(i < N) << "index " << i << " is greater than " << N;
        locks[i].lock();
        auto result = applyFunc(maps[i]);
        locks[i].unlock();
        return result;
    }

    template <class MapFunc> void map(MapFunc mapFunc) {
        for (auto i = 0u; i < N; i++) {
            locks[i].lock();
            mapFunc(maps[i]);
            locks[i].unlock();
        }
    }

    template <class T, class FoldFunc>
    auto fold(const T &firstValue, FoldFunc foldFunc) {
        T finalValue = firstValue;
        for (auto i = 0u; i < N; i++) {
            locks[i].lock();
            finalValue = foldFunc(finalValue, maps[i]);
            locks[i].unlock();
        }
        return finalValue;
    }

    //如果是同一个key，肯定对应同一个桶
    auto bucket_number(const KeyType &key) { return hasher(key) % N; }

private:
    HasherType hasher;
    HashMapType maps[N];
    HASH_MAP_LOCK_TYPE locks[N];
};
#endif //MYBLOCKCHAIN_HASHMAP_H
