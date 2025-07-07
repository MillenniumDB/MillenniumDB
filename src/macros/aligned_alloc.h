#pragma once

#include <cstddef>

constexpr std::size_t MDB_ALIGNMENT = 4096;

#ifdef _MSC_VER
    #include <malloc.h>
#else
    #include <cstdlib>
#endif

#ifdef _MSC_VER
    #define MDB_ALIGNED_ALLOC(SIZE) _aligned_malloc(SIZE, MDB_ALIGNMENT)
#else
    #define MDB_ALIGNED_ALLOC(SIZE) std::aligned_alloc(MDB_ALIGNMENT, SIZE)
#endif

#ifdef _MSC_VER
    #define MDB_ALIGNED_FREE _aligned_free
#else
    #define MDB_ALIGNED_FREE free
#endif
