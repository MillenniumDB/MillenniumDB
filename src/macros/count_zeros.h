#pragma once
#ifdef _MSC_VER
    #include <intrin.h>
#endif

#ifdef _MSC_VER
    #define MDB_COUNT_TRAILING_ZEROS_64 _tzcnt_u64
#else
    #define MDB_COUNT_TRAILING_ZEROS_64 __builtin_ctzll
#endif

#ifdef _MSC_VER
    #define MDB_COUNT_LEADING_ZEROS_64 __lzcnt64
#else
    #define MDB_COUNT_LEADING_ZEROS_64 __builtin_clzll
#endif
