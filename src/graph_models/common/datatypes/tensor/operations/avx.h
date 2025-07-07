#pragma once

#include "fallback.h"

#include <cstdint>
#include <immintrin.h>

namespace tensor::avx {

template<typename T>
struct AVXInterface;

template<>
struct AVXInterface<float> {
    using reg_type = __m256;
    using value_type = float;

    static constexpr std::size_t reg_width = 8;

    static inline reg_type load(const value_type* ptr)
    {
        return _mm256_load_ps(ptr);
    }

    static inline void store(value_type* ptr, reg_type va)
    {
        _mm256_store_ps(ptr, va);
    }

    static inline reg_type set_zero()
    {
        return _mm256_setzero_ps();
    }

    static inline reg_type set1(value_type f)
    {
        return _mm256_set1_ps(f);
    }

    static inline reg_type add(reg_type va, reg_type vb)
    {
        return _mm256_add_ps(va, vb);
    }

    static inline reg_type sub(reg_type va, reg_type vb)
    {
        return _mm256_sub_ps(va, vb);
    }

    static inline reg_type mul(reg_type va, reg_type vb)
    {
        return _mm256_mul_ps(va, vb);
    }

    static inline reg_type div(reg_type va, reg_type vb)
    {
        return _mm256_div_ps(va, vb);
    }

    static inline value_type hsum(reg_type v)
    {
        // v = [v7, v6, v5, v4, v3, v2, v1, v0]
        __m128 lo = _mm256_castps256_ps128(v);   // [v3, v2, v1, v0]
        __m128 hi = _mm256_extractf128_ps(v, 1); // [v7, v6, v5, v4]

        lo = _mm_add_ps(lo, hi);            // [v7+v3, v6+v2, v5+v1, v4+v0]
        __m128 shuf = _mm_movehdup_ps(lo);  // [v6+v2, v6+v2, v4+v0, v4+v0]
        __m128 sums = _mm_add_ps(lo, shuf); // [v7+v6+v3+v2, _, v5+v4+v1+v0, _]

        shuf = _mm_movehl_ps(shuf, sums); // [_, _, v7+v6+v3+v2, v5+v4+v1+v0]
        sums = _mm_add_ss(sums, shuf);    // [v7+v6+v5+v4+v3+v2+v1+v0, _, _, _]

        return _mm_cvtss_f32(sums);
    }

    static inline reg_type abs(reg_type v)
    {
        constexpr uint32_t mask = 0x7F'FF'FF'FF;
        const reg_type vmask = _mm256_castsi256_ps(_mm256_set1_epi32(mask));
        return _mm256_and_ps(v, vmask);
    }

    static inline reg_type neg(reg_type v)
    {
        const reg_type vmask = _mm256_set1_ps(-0.0f);
        return _mm256_xor_ps(v, vmask);
    }

    static inline reg_type ceil(reg_type v)
    {
        return _mm256_ceil_ps(v);
    }

    static inline reg_type floor(reg_type v)
    {
        return _mm256_floor_ps(v);
    }

    static inline reg_type inverse(reg_type v)
    {
        const reg_type recip = _mm256_rcp_ps(v);
        const reg_type corr = _mm256_sub_ps(_mm256_set1_ps(2.0f), _mm256_mul_ps(v, recip));
        return _mm256_mul_ps(recip, corr);
    }

    static inline reg_type sqrt(reg_type v)
    {
        return _mm256_sqrt_ps(v);
    }
};

template<>
struct AVXInterface<double> {
    using reg_type = __m256d;
    using value_type = double;

    static constexpr std::size_t reg_width = 4;

    static inline reg_type load(const value_type* ptr)
    {
        return _mm256_load_pd(ptr);
    }

    static inline void store(value_type* ptr, reg_type va)
    {
        _mm256_store_pd(ptr, va);
    }

    static inline reg_type set_zero()
    {
        return _mm256_setzero_pd();
    }

    static inline reg_type set1(value_type d)
    {
        return _mm256_set1_pd(d);
    }

    static inline reg_type add(reg_type va, reg_type vb)
    {
        return _mm256_add_pd(va, vb);
    }

    static inline reg_type sub(reg_type va, reg_type vb)
    {
        return _mm256_sub_pd(va, vb);
    }

    static inline reg_type mul(reg_type va, reg_type vb)
    {
        return _mm256_mul_pd(va, vb);
    }

    static inline reg_type div(reg_type va, reg_type vb)
    {
        return _mm256_div_pd(va, vb);
    }

    static inline value_type hsum(reg_type v)
    {
        // v = [v3, v2, v1, v0]
        __m128d lo = _mm256_castpd256_pd128(v);   // [v1, v0]
        __m128d hi = _mm256_extractf128_pd(v, 1); // [v3, v2]

        lo = _mm_add_pd(lo, hi);                    // [v3+v1, v2+v0]
        __m128d shuf = _mm_shuffle_pd(lo, lo, 0x1); // [v2+v0, v3+v1]
        __m128d sums = _mm_add_pd(lo, shuf);        // [v3+v2+v1+v0, v3+v2+v1+v0]

        return _mm_cvtsd_f64(sums);
    }

    static inline reg_type abs(reg_type v)
    {
        constexpr uint64_t mask = 0x7F'FF'FF'FF'FF'FF'FF'FF;
        const reg_type vmask = _mm256_castsi256_pd(_mm256_set1_epi64x(mask));
        return _mm256_and_pd(v, vmask);
    }

    static inline reg_type neg(reg_type v)
    {
        const reg_type vmask = _mm256_set1_pd(-0.0);
        return _mm256_xor_pd(v, vmask);
    }

    static inline reg_type ceil(reg_type v)
    {
        return _mm256_ceil_pd(v);
    }

    static inline reg_type floor(reg_type v)
    {
        return _mm256_floor_pd(v);
    }

    static inline reg_type inverse(reg_type v)
    {
        return _mm256_div_pd(_mm256_set1_pd(1.0), v);
    }

    static inline reg_type sqrt(reg_type v)
    {
        return _mm256_sqrt_pd(v);
    }
};

template<typename T>
struct AVXUtils {
    using size_type = std::size_t;
    using AVXI = AVXInterface<T>;

    template<typename VectorialOp, typename ScalarOp>
    static inline void
        binary_op(const T* lhs, const T* rhs, T* res, size_type n, VectorialOp vop, ScalarOp sop)
    {
        size_type i = 0;
        for (; i + AVXI::reg_width <= n; i += AVXI::reg_width) {
            const auto va = AVXI::load(lhs + i);
            const auto vb = AVXI::load(rhs + i);
            AVXI::store(res + i, vop(va, vb));
        }
        for (; i < n; ++i) {
            res[i] = sop(lhs[i], rhs[i]);
        }
    }

    template<typename VectorialOp, typename ScalarOp>
    static inline void unary_op(const T* ptr, T* res, size_type n, VectorialOp vop, ScalarOp sop)
    {
        size_type i = 0;
        for (; i + AVXI::reg_width <= n; i += AVXI::reg_width) {
            const auto v = AVXI::load(ptr + i);
            AVXI::store(res + i, vop(v));
        }
        for (; i < n; ++i) {
            res[i] = sop(ptr[i]);
        }
    }
};

template<typename T>
struct modifiers {
    using size_type = std::size_t;
    using AVXI = AVXInterface<T>;
    using AVXU = AVXUtils<T>;

    static inline void set(T* ptr, T val, size_type n)
    {
        const auto valv = AVXI::set1(val);

        AVXU::unary_op(
            ptr,
            ptr,
            n,
            [valv](auto) { return valv; },
            [val](auto) { return val; }
        );
    }

    static inline void neg(T* ptr, size_type n)
    {
        AVXU::unary_op(
            ptr,
            ptr,
            n,
            [](auto v) { return AVXI::neg(v); },
            [](auto x) { return -x; }
        );
    }

    static inline void abs(T* ptr, size_type n)
    {
        AVXU::unary_op(ptr, ptr, n, AVXI::abs, [](auto x) { return std::abs(x); });
    }

    static inline void ceil(T* ptr, size_type n)
    {
        AVXU::unary_op(ptr, ptr, n, AVXI::ceil, [](auto x) { return std::ceil(x); });
    }

    static inline void floor(T* ptr, size_type n)
    {
        AVXU::unary_op(ptr, ptr, n, AVXI::floor, [](auto x) { return std::floor(x); });
    }

    static inline void sparql11_round(T* ptr, size_type n)
    {
        fallback::modifiers<T>::sparql11_round(ptr, n);
    }

    static inline void inverse(T* ptr, size_type n)
    {
        AVXU::unary_op(ptr, ptr, n, AVXI::inverse, [](auto x) { return T { 1.0 } / x; });
    }

    static inline void sqrt(T* ptr, size_type n)
    {
        AVXU::unary_op(ptr, ptr, n, AVXI::sqrt, [](auto x) { return std::sqrt(x); });
    }
};

template<typename T>
struct vectorial_arithmetic {
    using size_type = std::size_t;
    using AVXI = AVXInterface<T>;
    using AVXU = AVXUtils<T>;

    static inline void add(const T* lhs, const T* rhs, T* res, size_type n)
    {
        AVXU::binary_op(lhs, rhs, res, n, AVXI::add, [](auto x, auto y) { return x + y; });
    }

    static inline void sub(const T* lhs, const T* rhs, T* res, size_type n)
    {
        AVXU::binary_op(lhs, rhs, res, n, AVXI::sub, [](auto x, auto y) { return x - y; });
    }

    static inline void mul(const T* lhs, const T* rhs, T* res, size_type n)
    {
        AVXU::binary_op(lhs, rhs, res, n, AVXI::mul, [](auto x, auto y) { return x * y; });
    }

    static inline void div(const T* lhs, const T* rhs, T* res, size_type n)
    {
        AVXU::binary_op(lhs, rhs, res, n, AVXI::div, [](auto x, auto y) { return x / y; });
    }
};

template<typename T>
struct scalar_arithmetic {
    using size_type = std::size_t;
    using AVXI = AVXInterface<T>;
    using AVXU = AVXUtils<T>;

    static inline void add(const T* lhs, T rhs, T* res, size_type n)
    {
        const auto vrhs = AVXI::set1(rhs);
        AVXU::unary_op(
            lhs,
            res,
            n,
            [vrhs](auto v) { return AVXI::add(v, vrhs); },
            [rhs](auto x) { return x + rhs; }
        );
    }

    static inline void sub(const T* lhs, T rhs, T* res, size_type n)
    {
        const auto vrhs = AVXI::set1(rhs);
        AVXU::unary_op(
            lhs,
            res,
            n,
            [vrhs](auto v) { return AVXI::sub(v, vrhs); },
            [rhs](auto x) { return x - rhs; }
        );
    }

    static inline void sub_lhs(T lhs, const T* rhs, T* res, size_type n)
    {
        const auto vlhs = AVXI::set1(lhs);
        AVXU::unary_op(
            rhs,
            res,
            n,
            [vlhs](auto v) { return AVXI::sub(vlhs, v); },
            [lhs](auto x) { return lhs - x; }
        );
    }

    static inline void mul(const T* lhs, T rhs, T* res, size_type n)
    {
        const auto vrhs = AVXI::set1(rhs);
        AVXU::unary_op(
            lhs,
            res,
            n,
            [vrhs](auto v) { return AVXI::mul(v, vrhs); },
            [rhs](auto x) { return x * rhs; }
        );
    }

    static inline void div(const T* lhs, T rhs, T* res, size_type n)
    {
        const auto vrhs = AVXI::set1(rhs);
        AVXU::unary_op(
            lhs,
            res,
            n,
            [vrhs](auto v) { return AVXI::div(v, vrhs); },
            [rhs](auto x) { return x / rhs; }
        );
    }

    static inline void div_lhs(T lhs, const T* rhs, T* res, size_type n)
    {
        const auto vlhs = AVXI::set1(lhs);
        AVXU::unary_op(
            rhs,
            res,
            n,
            [vlhs](auto v) { return AVXI::div(vlhs, v); },
            [lhs](auto x) { return lhs / x; }
        );
    }

    static inline void pow(const T* lhs, T rhs, T* res, size_type n)
    {
        return fallback::scalar_arithmetic<T>::pow(lhs, rhs, res, n);
    }
};

template<typename T>
struct computation {
    using size_type = std::size_t;
    using AVXI = AVXInterface<T>;

    static inline T sum(const T* ptr, size_type n)
    {
        auto sumv = AVXI::set_zero();

        size_type i = 0;
        for (; i + AVXI::reg_width < n; i += AVXI::reg_width) {
            const auto v = AVXI::load(ptr + i);
            sumv = AVXI::add(sumv, v);
        }

        T res = AVXI::hsum(sumv);

        for (; i < n; ++i) {
            res += ptr[i];
        }

        return res;
    }

    static inline T euclidean_distance(const T* lhs, const T* rhs, size_type n)
    {
        auto sumv = AVXI::set_zero();

        size_type i = 0;
        for (; i + AVXI::reg_width <= n; i += AVXI::reg_width) {
            const auto va = AVXI::load(lhs + i);
            const auto vb = AVXI::load(rhs + i);
            const auto diff = AVXI::sub(va, vb);
            const auto sq = AVXI::mul(diff, diff);
            sumv = AVXI::add(sumv, sq);
        }

        T res = AVXI::hsum(sumv);

        // Handle remaining elements
        for (; i < n; ++i) {
            const T diff = lhs[i] - rhs[i];
            res += diff * diff;
        }

        return std::sqrt(res);
    }

    static inline T manhattan_distance(const T* lhs, const T* rhs, size_type n)
    {
        auto sumv = AVXI::set_zero();

        size_type i = 0;
        for (; i + AVXI::reg_width <= n; i += AVXI::reg_width) {
            const auto va = AVXI::load(lhs + i);
            const auto vb = AVXI::load(rhs + i);
            const auto diff = AVXI::sub(va, vb);
            const auto abs_diff = AVXI::abs(diff);
            sumv = AVXI::add(sumv, abs_diff);
        }

        T res = AVXI::hsum(sumv);

        // Handle remaining elements
        for (; i < n; ++i) {
            res += std::abs(lhs[i] - rhs[i]);
        }

        return res;
    }

    static inline T cosine_similarity(const T* lhs, const T* rhs, size_type n)
    {
        auto aav = AVXI::set_zero();
        auto bbv = AVXI::set_zero();
        auto abv = AVXI::set_zero();

        size_type i = 0;
        for (; i + AVXI::reg_width <= n; i += AVXI::reg_width) {
            const auto va = AVXI::load(lhs + i);
            const auto vb = AVXI::load(rhs + i);
            aav = AVXI::add(aav, AVXI::mul(va, va));
            bbv = AVXI::add(bbv, AVXI::mul(vb, vb));
            abv = AVXI::add(abv, AVXI::mul(va, vb));
        }

        T aa = AVXI::hsum(aav);
        T bb = AVXI::hsum(bbv);
        T ab = AVXI::hsum(abv);

        for (; i < n; ++i) {
            aa += lhs[i] * lhs[i];
            bb += rhs[i] * rhs[i];
            ab += lhs[i] * rhs[i];
        }

        if (aa == 0 || bb == 0) {
            return T { -1.0 };
        }

        const T res = ab / std::sqrt(aa * bb);
        return std::min(T { 1.0 }, std::max(T { -1.0 }, res));
    }

    static inline T cosine_distance(const T* lhs, const T* rhs, size_type n)
    {
        return T { 1.0 } - cosine_similarity(lhs, rhs, n);
    }

    static inline T dot(const T* lhs, const T* rhs, size_type n)
    {
        auto resv = AVXI::set_zero();

        size_type i = 0;
        for (; i + AVXI::reg_width <= n; i += AVXI::reg_width) {
            const auto va = AVXI::load(lhs + i);
            const auto vb = AVXI::load(rhs + i);
            const auto mul = AVXI::mul(va, vb);
            resv = AVXI::add(resv, mul);
        }

        T res = AVXI::hsum(resv);

        for (; i < n; ++i) {
            res += lhs[i] * rhs[i];
        }

        return res;
    }
};

} // namespace tensor::avx
