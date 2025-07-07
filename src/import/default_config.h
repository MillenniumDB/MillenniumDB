#pragma once

#include <cassert>
#include <cstdint>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

namespace Import {
inline uint64_t get_file_size(const std::string& filepath)
{
    struct stat sb;
    if (stat(filepath.c_str(), &sb) == -1) {
        return 0;
    }
    return sb.st_size;
}

inline void make_multiple(uint64_t& value, uint64_t multiple)
{
    assert(multiple != 0);

    if (value < multiple) {
        value = multiple;
    } else if (value % multiple != 0) {
        value = ((value / multiple) + 1) * multiple;
    }
}

inline std::string get_suffix(uint64_t value)
{
    constexpr uint64_t size_KB = 1024ULL;
    constexpr uint64_t size_MB = 1024ULL * 1024;
    constexpr uint64_t size_GB = 1024ULL * 1024 * 1024;

    // up to 4 MB is expressed in KB
    if (value <= 4 * size_MB) {
        make_multiple(value, size_KB);
        return std::to_string(value / size_KB) + "KB";
    }
    // up to 4 GB is expressed in MB
    else if (value <= 4ULL * size_GB)
    {
        make_multiple(value, size_MB);
        return std::to_string(value / size_MB) + "MB";
    }
    // else expressed in GB
    else
    {
        make_multiple(value, size_GB);
        return std::to_string(value / (size_GB)) + "GB";
    }
}

} // namespace Import