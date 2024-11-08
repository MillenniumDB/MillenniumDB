#pragma once

#if (defined(__GNUC__) && (__GNUC__ >= 3))  \
  || (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 800)) \
  || defined(__clang__)
#    define MDB_likely(x) __builtin_expect(x, 1)
#    define MDB_unlikely(x) __builtin_expect(x, 0)
#else
#    define MDB_likely(x) (x)
#    define MDB_unlikely(x) (x)
#endif