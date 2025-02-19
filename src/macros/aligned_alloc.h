#pragma once
#ifdef _MSC_VER
    #include <malloc.h>
#else
    #include <cstdlib>
#endif

#ifdef _MSC_VER
    #define MDB_ALIGNED_ALLOC(SIZE) _aligned_malloc(SIZE, 4096)
    #define MDB_ALIGNED_ALLOC_EX(ALIGNMENT, SIZE) _aligned_malloc(SIZE, ALIGNMENT)
#else
    #define MDB_ALIGNED_ALLOC(SIZE) std::aligned_alloc(4096, SIZE)
    #define MDB_ALIGNED_ALLOC_EX(ALIGNMENT, SIZE) std::aligned_alloc(ALIGNMENT, SIZE)
#endif

#ifdef _MSC_VER
    #define MDB_ALIGNED_FREE _aligned_free
#else
    #define MDB_ALIGNED_FREE free
#endif