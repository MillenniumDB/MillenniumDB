#pragma once

#include <cassert>
#include <cstdint>
#include <ostream>
#include <string>

/**
 * Helper struct for logging text search index updates
 */

struct TextSearchIndexUpdateData {
    std::string text_search_index_name;

    uint_fast32_t inserted_elements { 0 };
    uint_fast32_t inserted_tokens { 0 };

    uint_fast32_t removed_elements { 0 };
    uint_fast32_t removed_tokens { 0 };

    friend std::ostream& operator<<(std::ostream& os, const TextSearchIndexUpdateData& data)
    {
        bool modified = false;
        os << "name                   : " << data.text_search_index_name << "\n";
        if (data.inserted_elements > 0) {
            assert(data.inserted_tokens > 0);
            os << "inserted_elements      : " << data.inserted_elements << "\n";
            os << "inserted_tokens        : " << data.inserted_tokens << "\n";
            modified = true;
        }
        if (data.removed_elements > 0) {
            assert(data.removed_tokens > 0);
            os << "removed_elements       : " << data.removed_elements << "\n";
            os << "removed_tokens         : " << data.removed_tokens << "\n";
            modified = true;
        }
        if (!modified) {
            os << "no insertions or removals\n";
        }
        return os;
    }
};