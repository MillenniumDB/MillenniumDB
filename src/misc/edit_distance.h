#pragma once

#include <cstdint>
#include <string>

#include "query/query_context.h"

inline uint64_t edit_distance(const std::string& lhs, const std::string& rhs)
{
    const auto lhs_size = lhs.size();
    const auto rhs_size = rhs.size();

    // Always make lhs the longer string
    if (lhs_size < rhs_size) {
        return edit_distance(rhs, lhs);
    }

    if (lhs_size == 0) {
        // empty lhs
        return rhs_size;
    }

    if (rhs_size == 0) {
        // empty rhs
        return lhs_size;
    }

    assert(lhs_size <= StringManager::MAX_STRING_SIZE && rhs_size <= StringManager::MAX_STRING_SIZE);

    auto prev = reinterpret_cast<uint64_t*>(get_query_ctx().get_buffer1());
    auto curr = reinterpret_cast<uint64_t*>(get_query_ctx().get_buffer2());

    // base case: transform empty string into any at size i
    for (uint64_t i = 0; i <= rhs_size; ++i) {
        prev[i] = i;
    }

    for (uint64_t i = 1; i <= lhs_size; ++i) {
        curr[0] = i;
        for (uint64_t j = 1; j <= rhs_size; ++j) {
            const int substitution_cost = (lhs[i - 1] == rhs[j - 1]) ? 0 : 1;
            curr[j] = std::min({
                prev[j] + 1, // delete
                curr[j - 1] + 1, // insert
                prev[j - 1] + substitution_cost // substitute
            });
        }
        std::swap(prev, curr);
    }

    return prev[rhs_size];
}
