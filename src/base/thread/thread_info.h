#pragma once

#include <chrono>
#include <cstdint>

struct ThreadInfo {
    bool interruption_requested = false;
    bool finished               = false;

    std::chrono::system_clock::time_point timeout;
};
