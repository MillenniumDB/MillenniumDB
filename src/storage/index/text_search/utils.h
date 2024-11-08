#pragma once

#include <cstddef>
#include <cstdint>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>


namespace TextSearch {


inline void write_bytes(unsigned char* ptr, size_t count, uint64_t value) {
    for (size_t i = 0, shift = 0; i < count; i++, shift += 8) {
        ptr[i] = (value >> shift) & 0xFF;
    }
}


inline uint64_t read_bytes(unsigned char* ptr, size_t count) {
    uint64_t value = 0;

    for (size_t i = 0, shift = 0; i < count; i++, shift += 8) {
        value |= static_cast<uint64_t>(ptr[i]) << shift;
    }

    return value;
}


inline std::string escape(const unsigned char* string, size_t size) {
    std::stringstream res;

    for (size_t i = 0; i < size; i++) {
        switch (string[i]) {
        case '\\': res << "\\\\";    break;
        case '\"': res << "\\\"";    break;
        case '\n': res << "\\\\n";   break;
        default: {
            if (string[i] >= 0b1000'0000) {
                res << "\\\\x" << std::hex << static_cast<int>(string[i]);
            } else {
                res << string[i];
            }
            break;
        }
        }
    }

    return res.str();
}


inline std::string escape(const std::string string) {
    return escape(reinterpret_cast<const unsigned char*>(string.c_str()), string.size());
}


template<class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T> vec) {
    os << "[";

    for (size_t i = 0; i < vec.size(); i++) {
        if (i != 0) os << ", ";
        os << std::to_string(vec[i]);
    }

    return os << "]";
}


inline std::pair<uint64_t, uint64_t> compress(uint64_t node_id, uint32_t score, uint64_t table_pointer) {
    uint64_t most_significant  = score >> 16;
    uint64_t least_significant = score & 0xFF'FF;

    node_id <<= 16;
    node_id |= most_significant;

    table_pointer |= least_significant << 48;

    return { node_id, table_pointer };
}


inline std::tuple<uint64_t, uint32_t, uint64_t> decompress(uint64_t node_id, uint64_t table_pointer) {
    uint32_t most_significant  = node_id & 0xFF'FF;
    uint32_t least_significant = (table_pointer >> 48) & 0xFF'FF;

    node_id >>= 16;
    table_pointer &= 0xFF'FF'FF'FF'FF'FF;

    uint32_t score = (most_significant << 16) | least_significant;

    return { node_id, score, table_pointer };
}


} // namespace TextSearch
