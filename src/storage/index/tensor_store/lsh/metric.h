#pragma once

#include <cassert>
#include <cmath>
#include <cstdint>
#include <vector>

#include "macros/likely.h"

namespace LSH {

enum class MetricType {
    ANGULAR,
    EUCLIDEAN,
    MANHATTAN,
};

class Metric {
public:
    static float euclidean_distance(const std::vector<float>& tensor1, const std::vector<float>& tensor2) {
        assert(tensor1.size() == tensor2.size());
        float res = 0.0f;
        #ifdef _OPENMP
        #pragma omp simd reduction(+ : res)
        #endif
        for (uint_fast32_t i = 0; i < tensor1.size(); ++i) {
            const auto diff = tensor1[i] - tensor2[i];
            res += diff * diff;
        }
        return sqrtf(res);
    }

    static float manhattan_distance(const std::vector<float>& tensor1, const std::vector<float>& tensor2) {
        assert(tensor1.size() == tensor2.size());
        float res = 0.0f;
        #ifdef _OPENMP
        #pragma omp simd reduction(+ : res)
        #endif
        for (uint_fast32_t i = 0; i < tensor1.size(); ++i) {
            const auto diff = tensor1[i] - tensor2[i];
            res += fabsf(diff);
        }
        return res;
    }

    static float cosine_distance(const std::vector<float>& tensor1, const std::vector<float>& tensor2) {
        assert(tensor1.size() == tensor2.size());
        float aa = 0.0f;
        float bb = 0.0f;
        float ab = 0.0f;

        for (uint_fast32_t i = 0; i < tensor1.size(); ++i) {
            aa += tensor1[i] * tensor1[i];
            bb += tensor2[i] * tensor2[i];
            ab += tensor1[i] * tensor2[i];
        }

        const auto denominator = aa * bb;
        // Due to floating point precision we could underflow 0.0f, so std::min is a must
        if (MDB_likely(denominator > 0.0f)) {
            return fminf(fmaxf(1.0f - ab / sqrtf(denominator), 0.0f), 1.0f);
        }

        // We consider the zero tensor perfectly dissimilar to any other
        return 1.0f;
    }
};
} // namespace LSH
