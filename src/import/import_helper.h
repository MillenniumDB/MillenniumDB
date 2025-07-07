#pragma once

#include <chrono>
#include <fstream>
#include <iostream>
#include <memory>

namespace Import {
inline std::unique_ptr<std::fstream> get_fstream(const std::string& filename)
{
    auto res = std::make_unique<std::fstream>();
    res->open(filename, std::ios::out | std::ios::app); // only to create new file
    res->close();
    res->open(filename, std::ios::in | std::ios::out | std::ios::binary);
    return res;
}

// prints the duration and updates the start time point
inline void print_duration(const std::string& msg, std::chrono::system_clock::time_point& start)
{
    auto end = std::chrono::system_clock::now();
    auto ms_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << msg << " duration: ";
    if (ms_duration < 1000) {
        std::cout << ms_duration << " milliseconds";
    } else if (ms_duration <= 60'000) { // less than 1 minute: print seconds
        std::cout << ms_duration / 1000.0 << " seconds";
    } else if (ms_duration <= 3'600'000) { // less than 1 hour: print minutes
        std::cout << (ms_duration / 60'000.0) << " minutes";
    } else {
        std::cout << (ms_duration / 3'600'000.0) << " hours";
    }
    std::cout << std::endl;
    start = end;
}

} // namespace Import
