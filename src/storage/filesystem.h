#pragma once

#include <experimental/filesystem>
#include <sys/stat.h>
#include <string>

namespace Filesystem {
    [[maybe_unused]]
    static bool exists(const std::string& str) {
        return std::experimental::filesystem::exists(str);
    }

    [[maybe_unused]]
    static bool is_empty(const std::string& str) {
        return std::experimental::filesystem::is_empty(str);
    }

    [[maybe_unused]]
    static bool is_directory(const std::string& str) {
        return std::experimental::filesystem::is_directory(str);
    }

    [[maybe_unused]]
    static void create_directories(const std::string& str) {
        std::experimental::filesystem::create_directories(str);
    }

    [[maybe_unused]]
    static uint64_t file_size(const std::string& file) {
        return std::experimental::filesystem::file_size(file);
    }
}
