#pragma once

#include "graph_models/common/datatypes/tensor/tensor.h"

#include <cstdint>
#include <ostream>

namespace HNSW {

enum class MetricType : uint8_t {
    EUCLIDEAN_DISTANCE,
    MANHATTAN_DISTANCE,
    COSINE_DISTANCE
};

inline std::string to_string(const MetricType& type)
{
    switch (type) {
    case MetricType::EUCLIDEAN_DISTANCE:
        return "EUCLIDEAN_DISTANCE";
    case MetricType::MANHATTAN_DISTANCE:
        return "MANHATTAN_DISTANCE";
    case MetricType::COSINE_DISTANCE:
        return "COSINE_DISTANCE";
    default:
        return "UNKNOWN_DISTANCE";
    }
}

inline std::ostream& operator<<(std::ostream& os, MetricType type)
{
    return os << to_string(type);
}

using MetricFuncType = float (*)(const tensor::Tensor<float>&, const tensor::Tensor<float>&);

// Helper function that maps MetricType to metric functions. If an unknown metric name is given, it returns nullptr
inline MetricFuncType metric_type2metric_func(MetricType metric_type)
{
    switch (metric_type) {
    case MetricType::EUCLIDEAN_DISTANCE:
        return tensor::Tensor<float>::euclidean_distance;
    case MetricType::MANHATTAN_DISTANCE:
        return tensor::Tensor<float>::manhattan_distance;
    case MetricType::COSINE_DISTANCE:
        return tensor::Tensor<float>::cosine_distance;
    default:
        return nullptr;
    }
}

} // namespace HNSW
