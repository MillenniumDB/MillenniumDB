#pragma once

#include <iostream>

template <typename T>
inline void _FATAL_ERROR(const T& t) {
    std::cerr << t;
    std::cerr << "\n\x1B[0m";
    std::exit(EXIT_FAILURE);
}

template <typename First, typename... Rest>
inline void _FATAL_ERROR(const First& first, const Rest&... rest) {
    std::cerr << first;
    _FATAL_ERROR(rest...); // recursive call using pack expansion syntax
}

template <typename First>
inline void FATAL_ERROR(const First& first) {
    std::cerr << "\x1B[0;31m";
    std::cerr << first;
    std::cerr << "\n\x1B[0m";
    std::exit(EXIT_FAILURE);
}

template <typename First, typename... Rest>
inline void FATAL_ERROR(const First& first, const Rest&... rest) {
    std::cerr << "\x1B[0;31m";
    std::cerr << first;
    _FATAL_ERROR(rest...); // recursive call using pack expansion syntax
}

template <typename T>
inline void _WARN(const T& t) {
    std::cerr << t;
    std::cerr << "\n\x1B[0m";
}

template <typename First, typename... Rest>
inline void _WARN(const First& first, const Rest&... rest) {
    std::cerr << first;
    _WARN(rest...); // recursive call using pack expansion syntax
}

template <typename First>
inline void WARN(const First& first) {
    std::cerr << "\x1b[0;33mWARNING: ";
    std::cerr << first;
    std::cerr << "\n\x1B[0m";
}

template <typename First, typename... Rest>
inline void WARN(const First& first, const Rest&... rest) {
    std::cerr << "\x1b[0;33mWARNING: ";
    std::cerr << first;
    _WARN(rest...); // recursive call using pack expansion syntax
}
