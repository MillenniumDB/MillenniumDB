#pragma once

#include <cstdint>
#include <ostream>
#include <string>

/**
 * Helper struct for logging text index updates
 */

struct TextIndexUpdateData {
    std::string index_name;

    bool created { false };

    uint_fast32_t inserted_elements { 0 };
    uint_fast32_t inserted_tokens { 0 };
    uint_fast32_t removed_elements { 0 };
    uint_fast32_t removed_tokens { 0 };

    friend std::ostream& operator<<(std::ostream& os, const TextIndexUpdateData& data)
    {
        os << "{\"name\": \"" << data.index_name << "\"";
        os << ", \"created\": " << (data.created ? "true" : "false");
        os << ", \"inserted_elements\": " << data.inserted_elements;
        os << ", \"inserted_tokens\": " << data.inserted_tokens;
        os << ", \"removed_elements\": " << data.removed_elements;
        os << ", \"removed_tokens\": " << data.removed_tokens;
        os << "}";
        return os;
    }
};
