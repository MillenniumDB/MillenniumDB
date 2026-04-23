#pragma once

#include <algorithm>
#include <cctype>
#include <charconv>
#include <cstdint>
#include <string>

namespace MdbBin {

inline std::string to_lower(const std::string& str)
{
    auto res = str;
    std::transform(res.begin(), res.end(), res.begin(), [](unsigned char c) {
        return std::tolower(c);
    });
    return res;
}

inline int64_t parse_bytes(const std::string& value)
{
    if (value.empty())
        return -1;

    int64_t number;
    // from_chars works great with the raw pointer range
    auto [ptr, ec] = std::from_chars(value.data(), value.data() + value.size(), number);

    // 1. Check if numeric parsing failed or number is negative
    if (ec != std::errc() || number < 0) {
        return -1;
    }

    // 2. Identify the suffix
    // 'ptr' now points to the first non-digit character
    std::string_view suffix(ptr, (value.data() + value.size()) - ptr);

    // 3. Remove leading/trailing whitespace if necessary, then handle units
    // (Note: std::from_chars does not skip leading whitespace)
    int64_t multiplier = 1;

    if (!suffix.empty()) {
        std::string suffix_lower = to_lower(std::string(suffix));

        if (suffix_lower == "b") {
            multiplier = 1;
        } else if (suffix_lower == "kb" || suffix_lower == "kib") {
            multiplier = 1024;
        } else if (suffix_lower == "mb" || suffix_lower == "mib") {
            multiplier = 1024 * 1024;
        } else if (suffix_lower == "gb" || suffix_lower == "gib") {
            multiplier = 1024LL * 1024 * 1024;
        } else if (suffix_lower == "tb" || suffix_lower == "tib") {
            multiplier = 1024LL * 1024 * 1024 * 1024;
        } else {
            return -1; // Unknown suffix
        }
    }

    // 4. Final calculation with overflow protection (optional but recommended)
    return number * multiplier;
}
} // namespace MdbBin
