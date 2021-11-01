#ifndef BASE__THREAD_INFO_H_
#define BASE__THREAD_INFO_H_

#include <cstdint>
#include <chrono>

struct ThreadInfo {
    bool interruption_requested = false;
    std::chrono::_V2::system_clock::time_point timeout;

    ThreadInfo(std::chrono::_V2::system_clock::time_point timeout) :
        timeout (timeout) { }
};

#endif // BASE__THREAD_INFO_H_
