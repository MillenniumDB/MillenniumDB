#pragma once

#include <algorithm>
#include <cmath>
#include <cstddef>

namespace tensor::fallback {

template<typename T>
struct modifiers {
    using size_type = std::size_t;

    static inline void set(T* ptr, T val, size_type n)
    {
        for (size_type i = 0; i < n; ++i) {
            ptr[i] = val;
        }
    }

    static inline void neg(T* ptr, size_type n)
    {
        for (size_type i = 0; i < n; ++i) {
            ptr[i] = -ptr[i];
        }
    }

    static inline void abs(T* ptr, size_type n)
    {
        for (size_type i = 0; i < n; ++i) {
            ptr[i] = std::abs(ptr[i]);
        }
    }

    static inline void ceil(T* ptr, size_type n)
    {
        for (size_type i = 0; i < n; ++i) {
            ptr[i] = std::ceil(ptr[i]);
        }
    }

    static inline void floor(T* ptr, size_type n)
    {
        for (size_type i = 0; i < n; ++i) {
            ptr[i] = std::floor(ptr[i]);
        }
    }

    static inline void sparql11_round(T* ptr, size_type n)
    {
        T whole;
        for (size_type i = 0; i < n; ++i) {
            const T frac = std::modf(ptr[i], &whole);
            ptr[i] = frac == T { -0.5 } ? whole : std::round(ptr[i]);
        }
    }

    static inline void inverse(T* ptr, size_type n)
    {
        for (size_type i = 0; i < n; ++i) {
            ptr[i] = T { 1.0 } / ptr[i];
        }
    }

    static inline void sqrt(T* ptr, size_type n)
    {
        for (size_type i = 0; i < n; ++i) {
            ptr[i] = std::sqrt(ptr[i]);
        }
    }
};

template<typename T>
struct vectorial_arithmetic {
    using size_type = std::size_t;

    static inline void add(const T* lhs, const T* rhs, T* res, size_type n)
    {
        for (size_type i = 0; i < n; ++i) {
            res[i] = lhs[i] + rhs[i];
        }
    }

    static inline void sub(const T* lhs, const T* rhs, T* res, size_type n)
    {
        for (size_type i = 0; i < n; ++i) {
            res[i] = lhs[i] - rhs[i];
        }
    }

    static inline void mul(const T* lhs, const T* rhs, T* res, size_type n)
    {
        for (size_type i = 0; i < n; ++i) {
            res[i] = lhs[i] * rhs[i];
        }
    }

    static inline void div(const T* lhs, const T* rhs, T* res, size_type n)
    {
        for (size_type i = 0; i < n; ++i) {
            res[i] = lhs[i] / rhs[i];
        }
    }
};

template<typename T>
struct scalar_arithmetic {
    using size_type = std::size_t;

    static inline void add(const T* lhs, T rhs, T* res, size_type n)
    {
        for (size_type i = 0; i < n; ++i) {
            res[i] = lhs[i] + rhs;
        }
    }

    static inline void sub(const T* lhs, T rhs, T* res, size_type n)
    {
        for (size_type i = 0; i < n; ++i) {
            res[i] = lhs[i] - rhs;
        }
    }

    static inline void sub_lhs(T lhs, const T* rhs, T* res, size_type n)
    {
        for (size_type i = 0; i < n; ++i) {
            res[i] = lhs - rhs[i];
        }
    }

    static inline void mul(const T* lhs, T rhs, T* res, size_type n)
    {
        for (size_type i = 0; i < n; ++i) {
            res[i] = lhs[i] * rhs;
        }
    }

    static inline void div(const T* lhs, T rhs, T* res, size_type n)
    {
        for (size_type i = 0; i < n; ++i) {
            res[i] = lhs[i] / rhs;
        }
    }

    static inline void div_lhs(T lhs, const T* rhs, T* res, size_type n)
    {
        for (size_type i = 0; i < n; ++i) {
            res[i] = lhs / rhs[i];
        }
    }

    static inline void pow(const T* lhs, T rhs, T* res, size_type n)
    {
        for (size_type i = 0; i < n; ++i) {
            res[i] = std::pow(lhs[i], rhs);
        }
    }
};

template<typename T>
struct computation {
    using size_type = std::size_t;

    static inline T sum(const T* ptr, size_type n)
    {
        T res { 0.0 };
        for (size_type i = 0; i < n; ++i) {
            res += ptr[i];
        }
        return res;
    }

    static inline T euclidean_distance(const T* lhs, const T* rhs, size_type n)
    {
        T res { 0.0 };
        for (size_type i = 0; i < n; ++i) {
            const T diff = lhs[i] - rhs[i];
            res += diff * diff;
        }
        return std::sqrt(res);
    }

    static inline T manhattan_distance(const T* lhs, const T* rhs, size_type n)
    {
        T res { 0.0 };
        for (size_type i = 0; i < n; ++i) {
            const T diff = lhs[i] - rhs[i];
            res += std::abs(diff);
        }
        return res;
    }

    static inline T cosine_similarity(const T* lhs, const T* rhs, size_type n)
    {
        T aa { 0.0 };
        T bb { 0.0 };
        T ab { 0.0 };

        for (size_type i = 0; i < n; ++i) {
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
        T res { 0.0 };
        for (size_type i = 0; i < n; ++i) {
            res += lhs[i] * rhs[i];
        }
        return res;
    }
};

template<typename T>
struct comparison {
    using size_type = std::size_t;

    static inline int cmp(const T* lhs, const T* rhs, size_type lhs_n, size_type rhs_n)
    {
        const auto min_size = std::min(lhs_n, rhs_n);
        for (size_type i = 0; i < min_size; ++i) {
            if (lhs[i] != rhs[i]) {
                return lhs[i] < rhs[i] ? -1 : 1;
            }
        }
        if (lhs_n != rhs_n) {
            return lhs_n < rhs_n ? -1 : 1;
        }
        return 0;
    }

    static inline bool eq(const T* lhs, const T* rhs, size_type lhs_n, size_type rhs_n)
    {
        if (lhs_n != rhs_n) {
            return false;
        }
        for (size_type i = 0; i < lhs_n; ++i) {
            if (lhs[i] != rhs[i]) {
                return false;
            }
        }
        return true;
    }

    static inline bool neq(const T* lhs, const T* rhs, size_type lhs_n, size_type rhs_n)
    {
        return !eq(lhs, rhs, lhs_n, rhs_n);
    }

    static inline bool lt(const T* lhs, const T* rhs, size_type lhs_n, size_type rhs_n)
    {
        return cmp(lhs, rhs, lhs_n, rhs_n) < 0;
    }

    static inline bool gt(const T* lhs, const T* rhs, size_type lhs_n, size_type rhs_n)
    {
        return cmp(lhs, rhs, lhs_n, rhs_n) > 0;
    }

    static inline bool leq(const T* lhs, const T* rhs, size_type lhs_n, size_type rhs_n)
    {
        return cmp(lhs, rhs, lhs_n, rhs_n) <= 0;
    }

    static inline bool geq(const T* lhs, const T* rhs, size_type lhs_n, size_type rhs_n)
    {
        return cmp(lhs, rhs, lhs_n, rhs_n) >= 0;
    }
};
} // namespace tensor::fallback
