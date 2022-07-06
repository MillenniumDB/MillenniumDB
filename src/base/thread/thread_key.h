#pragma once

#include <cstdint>

struct ThreadKey {
    uint64_t timestamp;
    uint64_t salt;

    ThreadKey(uint64_t timestamp, uint64_t salt) :
        timestamp (timestamp),
        salt      (salt) { }

    bool operator==(const ThreadKey& other) const {
        return timestamp == other.timestamp && salt == other.salt;
    }
};

struct ThreadKeyHasher {
    uint64_t operator()(const ThreadKey& thread_key) const {
        return thread_key.timestamp ^ thread_key.salt;
    }
};
