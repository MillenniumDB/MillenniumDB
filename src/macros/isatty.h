#pragma once

#include <cstdio>
#if defined(__linux__) || defined(__APPLE__)
    #include <unistd.h>
#elif defined(_WIN32)
    #include <io.h>
#endif

inline bool is_input_piped()
{
#if defined(__linux__) || defined(__APPLE__)
    return !isatty(fileno(stdin));
#elif defined(_WIN32)
    return !_isatty(_fileno(stdin));
#else
    return false; // Unknown platform
#endif
}
