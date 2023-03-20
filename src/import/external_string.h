#pragma once

#include <string>
#include <fstream>
#include <iostream>

#include "storage/page.h"
#include "storage/string_manager.h"
#include "third_party/xxhash/xxhash.h"

namespace Import {

class ExternalString {
public:
    // TODO: move constants to string manager?
    static constexpr size_t MIN_PAGE_REMAINING_BYTES = 9;
    static constexpr size_t MAX_LEN_BYTES = 5;

    static char* strings;

    uint64_t offset;

    ExternalString(uint64_t offset) : offset(offset) { }

    bool operator==(const ExternalString& other) const {
        auto p1 = strings + offset;
        auto p2 = strings + other.offset;

        size_t bytes_for_len1;
        size_t bytes_for_len2;

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
        size_t bytes_for_len;
        auto ptr = Import::ExternalString::strings + str.offset;
        size_t len = StringManager::get_string_len(ptr, &bytes_for_len);
        ptr += bytes_for_len;

        return XXH3_64bits(ptr, len);
    }
};
