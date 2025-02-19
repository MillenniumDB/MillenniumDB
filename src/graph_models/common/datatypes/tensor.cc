// TODO: Optimize vectorization by using aligned intrinsics explicitly (e.g. SSE, AVX)

#include "tensor.h"

#include <cassert>
#include <cmath>

template<typename T>
Tensor<T>::Tensor(std::vector<T, AlignedAllocator<T>>&& tensor_) :
    data_ { std::move(tensor_) }
{ }

template<typename T>
Tensor<T> Tensor<T>::from_literal(std::string_view literal, bool* error)
{
    // Creates a decimal from its string representation of the form [n0, n1, n2, ..., nN]
    *error = false;

    if (literal.empty()) {
        // Empty string
        *error = true;
        return Tensor<T>();
    }

    if (literal.front() != '[' || literal.back() != ']') {
        // Missing brackets
        *error = true;
        return Tensor<T>();
    }

    if (literal.size() == 2) {
        // Empty tensor
        return Tensor<T>();
    }

    std::vector<T, AlignedAllocator<T>> tensor;

    bool EXPECT_COMMA = false;
    std::size_t i = 1;
    while (i < literal.size() - 1) {
        // Skip whitespaces
        while (std::isspace(literal[i])) {
            ++i;
        }
        if (i == literal.size() - 1) {
            break;
        }

        // Verify separator/end
        if (EXPECT_COMMA) {
            if (literal[i] == ',') {
                ++i;
                EXPECT_COMMA = false;
                continue;
            } else {
                *error = true;
                return Tensor<T>();
            }
        }

        // Parse values
        try {
            std::size_t bytes_read = 0;
            if constexpr (std::is_same_v<T, float>) {
                tensor.emplace_back(std::stof(&literal[i], &bytes_read));
            } else if constexpr (std::is_same_v<T, double>) {
                tensor.emplace_back(std::stod(&literal[i], &bytes_read));
            } else {
                throw std::runtime_error("Unhandled tensor type");
            }
            i += bytes_read;
            EXPECT_COMMA = true;
        } catch (...) {
            *error = true;
            return Tensor<T>();
        }
    }

    return Tensor<T>(std::move(tensor));
}

template<typename T>
Tensor<T> Tensor<T>::from_external(const char* bytes, std::size_t size, bool* error)
{
    if (size % sizeof(T) != 0) {
        assert(false);
        *error = true;
        return Tensor<T>();
    }

    std::vector<T, AlignedAllocator<T>> tensor;
    tensor.resize(size / sizeof(T));
    std::memcpy(tensor.data(), bytes, size);
    return Tensor<T>(std::move(tensor));
}

template<typename T>
int Tensor<T>::compare(const Tensor<T>& lhs, const Tensor<T>& rhs)
{
    const auto min_size = std::min(lhs.size(), rhs.size());
    for (std::size_t i = 0; i < min_size; ++i) {
        if (lhs.data_[i] != rhs.data_[i]) {
            return lhs.data_[i] < rhs.data_[i] ? -1 : 1;
        }
    }
    if (lhs.size() != rhs.size()) {
        return lhs.size() < rhs.size() ? -1 : 1;
    }
    return 0;
}

template<typename T>
Tensor<T>& Tensor<T>::negate()
{
    for (std::size_t i = 0; i < size(); ++i) {
        data_[i] = -data_[i];
    }
    return *this;
}

template<typename T>
Tensor<T>& Tensor<T>::abs()
{
    for (std::size_t i = 0; i < size(); ++i) {
        data_[i] = std::abs(data_[i]);
    }
    return *this;
}

template<typename T>
Tensor<T>& Tensor<T>::ceil()
{
    for (std::size_t i = 0; i < size(); ++i) {
        data_[i] = std::ceil(data_[i]);
    }
    return *this;
}

template<typename T>
Tensor<T>& Tensor<T>::floor()
{
    for (std::size_t i = 0; i < size(); ++i) {
        data_[i] = std::floor(data_[i]);
    }
    return *this;
}

// Special case where SPARQL-1.1 ROUND != std::round
// SPARQL ROUND(-0.5f) == 0
// std::round(-0.5f) == -1
template<typename T>
Tensor<T>& Tensor<T>::sparql11_round()
{
    for (std::size_t i = 0; i < size(); ++i) {
        T whole;
        const T frac = std::modf(data_[i], &whole);
        if (frac == T(-0.5)) {
            data_[i] = whole;
        } else {
            data_[i] = std::round(data_[i]);
        }
    }
    return *this;
}

template<typename T>
Tensor<T>& Tensor<T>::multiplicative_inverse()
{
    for (std::size_t i = 0; i < size(); ++i) {
        data_[i] = T(1) / data_[i];
    }
    return *this;
}

template<typename T>
Tensor<T>& Tensor<T>::sqrt()
{
    for (std::size_t i = 0; i < size(); ++i) {
        data_[i] = std::sqrt(data_[i]);
    }
    return *this;
}

template<typename T>
Tensor<T>& Tensor<T>::add(const Tensor<T>& rhs)
{
    assert(size() == rhs.size());
    for (std::size_t i = 0; i < size(); ++i) {
        data_[i] += rhs.data_[i];
    }
    return *this;
}

template<typename T>
Tensor<T>& Tensor<T>::subtract(const Tensor<T>& rhs)
{
    assert(size() == rhs.size());
    for (std::size_t i = 0; i < size(); ++i) {
        data_[i] -= rhs.data_[i];
    }
    return *this;
}

template<typename T>
Tensor<T>& Tensor<T>::multiply(const Tensor<T>& rhs)
{
    assert(size() == rhs.size());
    for (std::size_t i = 0; i < size(); ++i) {
        data_[i] *= rhs.data_[i];
    }
    return *this;
}

template<typename T>
Tensor<T>& Tensor<T>::divide(const Tensor<T>& rhs)
{
    assert(size() == rhs.size());
    for (std::size_t i = 0; i < size(); ++i) {
        data_[i] /= rhs.data_[i];
    }
    return *this;
}

template<typename T>
T Tensor<T>::dot(const Tensor<T>& rhs) const
{
    assert(size() == rhs.size());
    T sum { 0 };
    for (std::size_t i = 0; i < size(); ++i) {
        sum += data_[i] * rhs.data_[i];
    }
    return sum;
}

template<typename T>
Tensor<T>& Tensor<T>::add(T rhs)
{
    for (std::size_t i = 0; i < size(); ++i) {
        data_[i] += rhs;
    }
    return *this;
}

template<typename T>
Tensor<T>& Tensor<T>::subtract(T rhs)
{
    for (std::size_t i = 0; i < size(); ++i) {
        data_[i] -= rhs;
    }
    return *this;
}

template<typename T>
Tensor<T>& Tensor<T>::multiply(T rhs)
{
    for (std::size_t i = 0; i < size(); ++i) {
        data_[i] *= rhs;
    }
    return *this;
}

template<typename T>
Tensor<T>& Tensor<T>::divide(T rhs)
{
    for (std::size_t i = 0; i < size(); ++i) {
        data_[i] /= rhs;
    }
    return *this;
}

template<typename T>
Tensor<T>& Tensor<T>::pow(T rhs)
{
    for (std::size_t i = 0; i < size(); ++i) {
        data_[i] = std::pow(data_[i], rhs);
    }
    return *this;
}

template<typename T>
T Tensor<T>::sum() const
{
    T sum { 0 };
    for (std::size_t i = 0; i < size(); ++i) {
        sum += data_[i];
    }
    return sum;
}

template<typename T>
T Tensor<T>::euclidean_distance(const Tensor<T>& rhs) const
{
    assert(size() == rhs.size());
    T sum { 0 };
    for (std::size_t i = 0; i < size(); ++i) {
        const auto diff = data_[i] - rhs.data_[i];
        sum += diff * diff;
    }
    return std::sqrt(sum);
}

template<typename T>
T Tensor<T>::manhattan_distance(const Tensor<T>& rhs) const
{
    assert(size() == rhs.size());
    T sum { 0 };
    for (std::size_t i = 0; i < size(); ++i) {
        const auto diff = data_[i] - rhs.data_[i];
        sum += std::abs(diff);
    }
    return sum;
}

template<typename T>
T Tensor<T>::cosine_similarity(const Tensor<T>& rhs) const
{
    assert(size() == rhs.size());
    T ab { 0 };
    T aa { 0 };
    T bb { 0 };
    for (std::size_t i = 0; i < size(); ++i) {
        ab += data_[i] * rhs.data_[i];
        aa += data_[i] * data_[i];
        bb += rhs.data_[i] * rhs.data_[i];
    }
    const auto denominator = aa * bb;
    if (denominator > 0) {
        // Due to floating point precision we could underflow -1.0 or overflow 1.0, so std::min and std::max is a must
        return std::min(std::max(ab / std::sqrt(denominator), T(-1)), T(1));
    }

    // We consider the zero tensor perfectly dissimilar to any other
    return T(-1);
}

template class Tensor<float>;
template class Tensor<double>;
