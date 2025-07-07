#pragma once

#include <cassert>
#include <cstddef>
#include <new>

#if defined(_MSC_VER)
#include <malloc.h>
#define TENSOR_ALIGNED_ALLOC(size, align) _aligned_malloc(size, align);
#define TENSOR_ALIGNED_FREE(ptr) _aligned_free(ptr);
#else
#include <cstdlib>
#define TENSOR_ALIGNED_ALLOC(size, align) std::aligned_alloc(align, size)
#define TENSOR_ALIGNED_FREE(ptr) std::free(ptr)
#endif

namespace tensor {

template<typename T, std::size_t alignment>
struct aligned_allocator {
    using value_type = T;
    using size_type = std::size_t;

    static_assert(alignment >= sizeof(value_type), "alignment must be at least sizeof(T)");

    [[nodiscard]] static inline T* allocate(size_type n)
    {
        assert(((sizeof(T) * n) % alignment) == 0 && "num_bytes must be multiple of alignment");
        void* ptr = TENSOR_ALIGNED_ALLOC(sizeof(T) * n, alignment);
        if (!ptr) {
            throw std::bad_alloc();
        }
        return reinterpret_cast<value_type*>(ptr);
    }

    static inline void deallocate(value_type* ptr)
    {
        TENSOR_ALIGNED_FREE(ptr);
    }
};
} // namespace tensor
