#pragma once

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <string>

namespace MdbBin {

inline std::string to_lower(const std::string& str)
{
    auto res = str;
    std::transform(res.begin(), res.end(), res.begin(), [](unsigned char c) { return std::tolower(c); });
    return res;
}

inline int64_t parse_bytes(const std::string& value)
{
    size_t chars_read;
    int64_t multiplier = 1;
    try {
        auto number = std::stoll(value, &chars_read);
        if (number >= 0) {
            if (chars_read < value.size()) {
                auto suffix = to_lower(value.substr(chars_read, value.size() - chars_read));
                if (suffix == "b") {
                    multiplier = 1;
                } else if (suffix == "kb" || suffix == "kib") {
                    multiplier = 1024;
                } else if (suffix == "mb" || suffix == "mib") {
                    multiplier = 1024 * 1024;
                } else if (suffix == "gb" || suffix == "gib") {
                    multiplier = 1024 * 1024 * 1024;
                } else if (suffix == "tb" || suffix == "tib") {
                    multiplier = 1024LL * 1024 * 1024 * 1024;
                } else {
                    return -1;
                }
            }
            return multiplier * number;
        }
    } catch (...) {
    }
    return -1;
}
} // namespace MdbBin
