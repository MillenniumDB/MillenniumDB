#pragma once

#include <chrono>
#include <cstdint>

struct ThreadInfo {
    bool interruption_requested = false;
    std::chrono::system_clock::time_point timeout;

    ThreadInfo(std::chrono::system_clock::time_point timeout) : timeout(timeout) { }
};
