#pragma once

#include <cstring>

#include "system/string_manager.h"
#include "third_party/hashes/hash_function_wrapper.h"

namespace Import {

class ExternalString {
public:
    // we suppose no string will need more than these bytes to encode its length
    static constexpr size_t MAX_LEN_BYTES = 5;

    static char* strings;

    uint64_t offset;

    ExternalString(uint64_t offset) : offset(offset) { }

    bool operator==(const ExternalString& other) const {
        auto p1 = strings + offset;
        auto p2 = strings + other.offset;

        uint64_t bytes_for_len1;
        uint64_t bytes_for_len2;

        size_t len1 = StringManager::get_string_len(p1, &bytes_for_len1);
        size_t len2 = StringManager::get_string_len(p2, &bytes_for_len2);

        if (len1 != len2) return false;

        p1 += bytes_for_len1;
        p2 += bytes_for_len2;

        return strncmp(p1, p2, len1) == 0;
    }
};

} // namespace Import

template<>
struct std::hash<Import::ExternalString> {
    size_t operator()(const Import::ExternalString& str) const {
        uint64_t bytes_for_len;
        auto ptr = Import::ExternalString::strings + str.offset;
        size_t len = StringManager::get_string_len(ptr, &bytes_for_len);
        ptr += bytes_for_len;

        return HashFunctionWrapper(ptr, len);
    }
};
