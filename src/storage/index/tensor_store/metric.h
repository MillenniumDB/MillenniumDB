#pragma once

#include <cassert>
#include <cmath>
#include <cstdint>
#include <iostream>

#include "macros/likely.h"

namespace Metric {
using MetricFuncType = float(const float*, const float*, std::size_t);

enum class MetricType : uint8_t {
    ANGULAR,
    EUCLIDEAN,
    MANHATTAN,
};

inline std::ostream& operator<<(std::ostream& os, MetricType metric_type)
{
    switch (metric_type) {
    case MetricType::ANGULAR:
        return os << "ANGULAR";
    case MetricType::EUCLIDEAN:
        return os << "EUCLIDEAN";
    case MetricType::MANHATTAN:
        return os << "MANHATTAN";
    default:
        return os << "UNKNOWN";
    }
}

inline float euclidean_distance(const float* tensor1, const float* tensor2, std::size_t dim)
{
    float res = 0.0f;
#ifdef _OPENMP
#pragma omp simd reduction(+ : res)
#endif
    for (uint64_t i = 0; i < dim; ++i) {
        const auto diff = tensor1[i] - tensor2[i];
        res += diff * diff;
    }
    return sqrtf(res);
}

inline float manhattan_distance(const float* tensor1, const float* tensor2, std::size_t dim)
{
    float res = 0.0f;
#ifdef _OPENMP
#pragma omp simd reduction(+ : res)
#endif
    for (uint64_t i = 0; i < dim; ++i) {
        const auto diff = tensor1[i] - tensor2[i];
        res += fabsf(diff);
    }
    return res;
}

inline float cosine_distance(const float* tensor1, const float* tensor2, std::size_t dim)
{
    float ab = 0.0f;
    float aa = 0.0f;
    float bb = 0.0f;

#ifdef _OPENMP
#pragma omp simd reduction(+ : ab, aa, bb)
#endif
    for (uint64_t i = 0; i < dim; ++i) {
        ab += tensor1[i] * tensor2[i];
        aa += tensor1[i] * tensor1[i];
        bb += tensor2[i] * tensor2[i];
    }

    const auto denominator = aa * bb;
    // Due to floating point precision we could underflow 0.0f, so std::min is a must
    if (MDB_likely(denominator > 0.0f)) {
        return fminf(fmaxf(1.0f - ab / sqrtf(denominator), 0.0f), 1.0f);
    }

    // We consider the zero tensor perfectly dissimilar to any other
    return 1.0f;
}

inline MetricFuncType* get_metric_function(MetricType metric_type)
{
    switch (metric_type) {
    case MetricType::ANGULAR:
        return &cosine_distance;
    case MetricType::EUCLIDEAN:
        return &euclidean_distance;
    case MetricType::MANHATTAN:
        return &manhattan_distance;
    default:
        return nullptr;
    }
}
} // namespace Metric
