#pragma once

#include <cstdint>
#include <ostream>
#include <string>

/**
 * Helper struct for logging hnsw index updates
 */

struct HNSWIndexUpdateData {
    std::string index_name;

    bool created { false };

    uint_fast32_t inserted_elements { 0 };
    uint_fast32_t removed_elements { 0 };

    friend std::ostream& operator<<(std::ostream& os, const HNSWIndexUpdateData& data)
    {
        os << "{\"name\": \"" << data.index_name << "\"";
        os << ", \"created\": " << (data.created ? "true" : "false");
        os << ", \"inserted_elements\": " << data.inserted_elements;
        os << ", \"removed_elements\": " << data.removed_elements;
        os << "}";
        return os;
    }
};
