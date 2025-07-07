#pragma once

#include "fallback.h"

#if defined(__AVX__)
#include "avx.h"
#define SIMD_ALIGNMENT_BYTES 32
#else
#define SIMD_ALIGNMENT_BYTES 8
#endif

namespace tensor {

struct operations {
    static constexpr std::size_t alignment_bytes = SIMD_ALIGNMENT_BYTES;

    template<typename T>
    using comparison = fallback::comparison<T>;

#if defined(__AVX__)
    template<typename T>
    using modifiers = avx::modifiers<T>;

    template<typename T>
    using vectorial_arithmetic = avx::vectorial_arithmetic<T>;

    template<typename T>
    using scalar_arithmetic = avx::scalar_arithmetic<T>;

    template<typename T>
    using computation = avx::computation<T>;
#else
    template<typename T>
    using modifiers = fallback::modifiers<T>;

    template<typename T>
    using vectorial_arithmetic = fallback::vectorial_arithmetic<T>;

    template<typename T>
    using scalar_arithmetic = fallback::scalar_arithmetic<T>;

    template<typename T>
    using computation = fallback::computation<T>;
#endif
};
} // namespace tensor
