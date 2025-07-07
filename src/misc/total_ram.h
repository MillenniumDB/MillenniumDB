#pragma once

#include <cstdint>

#ifdef __APPLE__
#include <sys/sysctl.h>

inline uint64_t get_total_ram()
{
    int mib[2] = { CTL_HW, HW_MEMSIZE };
    uint64_t total_RAM = 0;
    size_t length = sizeof(total_RAM);

    // Get the physical memory size
    if (sysctl(mib, 2, &total_RAM, &length, NULL, 0) == -1) {
        return 0;
    }

    return total_RAM;
}
#elif _WIN32
#include <windows.h>

inline uint64_t get_total_ram()
{
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    return memInfo.ullTotalPhys;
}
#else
#include <sys/sysinfo.h>

inline uint64_t get_total_ram()
{
    struct sysinfo system_info;
    if (sysinfo(&system_info) != 0) {
        // Error reading the system info
        return;
    }

    return system_info.totalram;
}
#endif
