//
// Created by peng on 2021/3/23.
//
#ifndef NEUBLOCKCHAIN_SHA256_HEPLER_H
#define NEUBLOCKCHAIN_SHA256_HEPLER_H

#include "common/sha224_256.h"
#include <string>

class SHA256Helper {
public:
    inline SHA256Helper() = default;

    inline bool reset() {
        if (SHA256Reset(&ctx) != shaSuccess)
            return false;
        return true;
    }

    inline bool hash(const std::string& data, std::string* digest) {
        if (!reset())
            return false;
        if(!append(data))
            return false;
        return execute(digest);
    }

    inline bool hash(const std::string& data) {
        if (!reset())
            return false;
        return append(data);
    }

    inline bool append(const std::string& data) {
        if (SHA256Input(&ctx, reinterpret_cast<const uint8_t *>(data.data()), data.size()) != shaSuccess)
            return false;
        return true;
    }

    inline bool execute(std::string* digest) {
        digest->clear();
        digest->resize(SHA256HashSize);
        if(SHA256Result(&ctx, reinterpret_cast<uint8_t *>(digest->data())) != shaSuccess)
            return false;
        return true;
    }

private:
    SHA256Context ctx{};

};

#endif //NEUBLOCKCHAIN_SHA256_HEPLER_H
