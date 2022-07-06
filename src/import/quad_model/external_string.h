#pragma once

#include "third_party/xxhash/xxhash.h"

namespace Import {
class ExternalString {
public:
    static char* strings;
    uint64_t offset;

    ExternalString(uint64_t offset) : offset(offset) { }

    bool operator==(const ExternalString& other) const {
        return strcmp(strings + offset, strings + other.offset) == 0;
    }
};

struct ExternalStringHasher {
    size_t operator()(const ExternalString& str) const {
        size_t str_len = strlen(ExternalString::strings + str.offset);
        return XXH3_64bits(ExternalString::strings + str.offset, str_len);
    }
};
}
