#pragma once

#include <cstddef>
#include <limits>
#include <new>

#include "macros/aligned_alloc.h"
#include "macros/likely.h"

template<typename T>
class AlignedAllocator {
public:
    using value_type = T;

    // 64 bytes is sufficient for AVX-512 and most other SIMD instructions
    static constexpr std::size_t DEFAULT_ALIGNMENT = 64;

    static_assert(DEFAULT_ALIGNMENT >= alignof(value_type), "Alignment must be at least alignof(T)");
    static_assert(DEFAULT_ALIGNMENT % alignof(value_type) == 0, "Alignment must be a multiple of alignof(T)");
    static_assert(DEFAULT_ALIGNMENT % sizeof(void*) == 0, "Alignment must be a multiple of sizeof(void*)");

    constexpr AlignedAllocator() noexcept { }

    template<class U>
    constexpr AlignedAllocator(const AlignedAllocator<U>&) noexcept
    { }

    template<class U>
    constexpr bool operator==(const AlignedAllocator<U>&) const noexcept
    {
        return true;
    }

    template<class U>
    constexpr bool operator!=(const AlignedAllocator<U>&) const noexcept
    {
        return false;
    }

    [[nodiscard]] inline value_type* allocate(std::size_t n) const
    {
        const auto min_alloc = sizeof(T) * n;
        const auto alloc_remainder = min_alloc % DEFAULT_ALIGNMENT;
        const auto alloc_size = (alloc_remainder == 0) ? min_alloc
                                                       : (min_alloc + (DEFAULT_ALIGNMENT - alloc_remainder));
        void* ptr = MDB_ALIGNED_ALLOC_EX(DEFAULT_ALIGNMENT, alloc_size);

        if (MDB_unlikely(ptr == nullptr)) {
            throw std::bad_alloc();
        }

        return static_cast<value_type*>(ptr);
    }

    inline void deallocate(T* const ptr, [[maybe_unused]] std::size_t n) const noexcept
    {
        if (MDB_likely(ptr != nullptr)) {
            MDB_ALIGNED_FREE(ptr);
        }
    }
};