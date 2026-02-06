#pragma once

#include <chrono>
#include <ctime>
#include <string>

#if defined(__linux__) || defined(__APPLE__)
#define MDB_GMTIME(x, y) gmtime_r(x, y)
#elif defined(_WIN32)
#define MDB_GMTIME(x, y) gmtime_s(y, x)
#endif

// always should use mdb_format_time to work with times as it is thread safe
// the method std::localtime is not thread safe
inline std::string mdb_format_time(std::chrono::system_clock::time_point time, const char* fmt)
{
    std::time_t t = std::chrono::system_clock::to_time_t(time);
    struct tm gmt;
    MDB_GMTIME(&t, &gmt);
    char time_buf[64];
    std::strftime(time_buf, sizeof(time_buf), fmt, &gmt);
    return std::string(time_buf);
}
