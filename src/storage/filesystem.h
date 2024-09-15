#pragma once

#include <filesystem>
#include <string>

namespace Filesystem {
    [[maybe_unused]]
    static bool exists(const std::string& str) {
        return std::filesystem::exists(str);
    }

    [[maybe_unused]]
    static bool is_empty(const std::string& str) {
        return std::filesystem::is_empty(str);
    }

    [[maybe_unused]]
    static bool is_directory(const std::string& str) {
        return std::filesystem::is_directory(str);
    }

    [[maybe_unused]]
    static bool is_regular_file(const std::string& str) {
        return std::filesystem::is_regular_file(str);
    }

    [[maybe_unused]]
    static void create_directories(const std::string& str) {
        std::filesystem::create_directories(str);
    }

    [[maybe_unused]]
    static uint64_t file_size(const std::string& file) {
        return std::filesystem::file_size(file);
    }

    [[maybe_unused]]
    static std::string get_extension(const std::string& path) {
        return std::filesystem::path(path).extension().string();
    }

    [[maybe_unused]]
    static std::string replace_extension(const std::string& path, const std::string& ext) {
        return std::filesystem::path(path).replace_extension(ext);
    }

    [[maybe_unused]]
    static std::uintmax_t remove_all( const std::filesystem::path& p ) {
        return std::filesystem::remove_all(p);
    }

    [[maybe_unused]]
    static std::filesystem::directory_iterator directory_iterator(const std::string& path) {
        return std::filesystem::directory_iterator(path);
    }
}
