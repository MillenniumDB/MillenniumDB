#pragma once

#include <cstddef>
#include <cstring>
#include <stdexcept>
#include <string>
#include <vector>

#include "graph_models/object_id.h"
#include "misc/aligned_allocator.h"

template<typename T>
class Tensor {
public:
    // Empty tensor
    explicit Tensor() = default;

    // From data
    explicit Tensor(std::vector<T, AlignedAllocator<T>>&& data);
    // From string literal
    static Tensor<T> from_literal(std::string_view literal, bool* error);
    // From external bytes
    static Tensor<T> from_external(const char* bytes, std::size_t size, bool* error);

    static int compare(const Tensor& lhs, const Tensor& rhs);

    // Getters for the corresponding masks given T
    static constexpr uint64_t get_inline_mask()
    {
        if constexpr (std::is_same_v<T, float>) {
            return ObjectId::MASK_TENSOR_FLOAT_INLINED;
        } else if constexpr (std::is_same_v<T, double>) {
            return ObjectId::MASK_TENSOR_DOUBLE_INLINED;
        } else {
            throw std::runtime_error("Unhandled tensor type");
        }
    }

    static constexpr uint64_t get_external_mask()
    {
        if constexpr (std::is_same_v<T, float>) {
            return ObjectId::MASK_TENSOR_FLOAT_EXTERN;
        } else if constexpr (std::is_same_v<T, double>) {
            return ObjectId::MASK_TENSOR_DOUBLE_EXTERN;
        } else {
            throw std::runtime_error("Unhandled tensor type");
        }
    }

    static constexpr uint64_t get_tmp_mask()
    {
        if constexpr (std::is_same_v<T, float>) {
            return ObjectId::MASK_TENSOR_FLOAT_TMP;
        } else if constexpr (std::is_same_v<T, double>) {
            return ObjectId::MASK_TENSOR_DOUBLE_TMP;
        } else {
            throw std::runtime_error("Unhandled tensor type");
        }
    }

    ~Tensor() = default;

    constexpr bool operator==(const Tensor& rhs) const
    {
        if (size() != rhs.size()) {
            return false;
        }
        for (std::size_t i = 0; i < size(); ++i) {
            if (data_[i] != rhs.data_[i]) {
                return false;
            }
        }
        return true;
    }

    constexpr bool operator<(const Tensor& rhs) const
    {
        const auto min_size = std::min(size(), rhs.size());
        for (std::size_t i = 0; i < min_size; ++i) {
            if (data_[i] != rhs.data_[i]) {
                return data_[i] < rhs.data_[i];
            }
        }
        return data_ > rhs.data_;
    }

    constexpr bool operator!=(const Tensor& rhs) const
    {
        return !(*this == rhs);
    }

    constexpr bool operator>(const Tensor& rhs) const
    {
        return !(*this < rhs) && *this != rhs;
    }

    constexpr bool operator>=(const Tensor& rhs) const
    {
        return *this > rhs || *this == rhs;
    }

    constexpr bool operator<=(const Tensor& rhs) const
    {
        return *this < rhs || *this == rhs;
    }

    T& operator[](std::size_t index)
    {
        return data_[index];
    }

    const T& operator[](std::size_t index) const
    {
        return data_[index];
    }

    // Modifiers
    Tensor& negate();
    Tensor& abs();
    Tensor& ceil();
    Tensor& floor();
    Tensor& sparql11_round();
    Tensor& multiplicative_inverse();
    Tensor& sqrt();

    // Tensor arithmetic
    Tensor& add(const Tensor& rhs);
    Tensor& subtract(const Tensor& rhs);
    Tensor& multiply(const Tensor& rhs);
    Tensor& divide(const Tensor& rhs);
    T dot(const Tensor& rhs) const;

    // Scalar arithmetic
    Tensor& add(T rhs);
    Tensor& subtract(T rhs);
    Tensor& multiply(T rhs);
    Tensor& divide(T rhs);
    Tensor& pow(T rhs);

    // Computations
    T sum() const;
    T euclidean_distance(const Tensor& rhs) const;
    T manhattan_distance(const Tensor& rhs) const;
    T cosine_similarity(const Tensor& rhs) const;

    constexpr T* data() {
        return data_.data();
    }

    constexpr const T* data() const
    {
        return data_.data();
    }

    constexpr std::size_t size() const
    {
        return data_.size();
    }

    constexpr bool empty() const
    {
        return data_.empty();
    }

    std::string to_string() const
    {
        if (empty()) {
            return "[]";
        }
        std::string str = "[";
        str += std::to_string(data_[0]);
        for (std::size_t i = 1; i < size(); ++i) {
            str += ", " + std::to_string(data_[i]);
        }
        str += "]";
        return str;
    }

private:
    std::vector<T, AlignedAllocator<T>> data_;
};

template<typename T>
std::ostream& operator<<(std::ostream& os, const Tensor<T>& tensor)
{
    return os << tensor.to_string();
}
