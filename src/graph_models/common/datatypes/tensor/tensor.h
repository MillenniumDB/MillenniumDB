#pragma once

#include <cstddef>
#include <cstring>
#include <initializer_list>
#include <iostream>
#include <string>
#include <type_traits>

#include "graph_models/object_id.h"
#include "operations/operations.h"
#include "tensor_allocator.h"

namespace tensor {

// stl-like floating point container, optimized for vectorial operations
template<typename T>
class Tensor {
    static_assert(std::is_floating_point_v<T>, "T must be a floating point");

public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;
    using allocator = aligned_allocator<T, operations::alignment_bytes>;

    // ============================================================================
    // Constructors / Destructor
    // ============================================================================
    Tensor() noexcept :
        size_ { 0 },
        capacity_ { 0 },
        data_ { nullptr }
    { }

    Tensor(size_type n, value_type val = value_type()) :
        size_ { n },
        capacity_ { round_up_allocation_size(size_) },
        data_ { allocator::allocate(capacity_) }
    {
        set(val);
    }

    template<class InputIterator>
    Tensor(InputIterator first, InputIterator last)
    {
        size_ = static_cast<size_type>(std::distance(first, last));

        if (size_ == 0) {
            capacity_ = 0;
            data_ = nullptr;
            return;
        }

        capacity_ = round_up_allocation_size(size_);
        data_ = allocator::allocate(capacity_);

        size_type i = 0;
        for (auto it = first; it != last; ++it, ++i) {
            data_[i] = *it;
        }
    }

    Tensor(const char* bytes, size_type num_bytes) :
        size_ { num_bytes / sizeof(value_type) },
        capacity_ { round_up_allocation_size(size_) },
        data_ { allocator::allocate(capacity_) }
    {
        std::memcpy(data_, bytes, num_bytes);
    }

    Tensor(const Tensor& other) :
        size_ { other.size_ },
        capacity_ { round_up_allocation_size(size_) },
        data_ { allocator::allocate(capacity_) }
    {
        std::memcpy(data_, other.data_, sizeof(value_type) * size_);
    }

    Tensor& operator=(const Tensor& other)
    {
        if (this == &other) {
            return *this;
        }
        allocator::deallocate(data_);
        size_ = other.size_;
        capacity_ = round_up_allocation_size(size_);
        data_ = allocator::allocate(capacity_);
        std::memcpy(data_, other.data_, sizeof(value_type) * size_);
    }

    Tensor(Tensor&& other) noexcept :
        size_ { other.size_ },
        capacity_ { other.capacity_ },
        data_ { other.data_ }
    {
        other.size_ = 0;
        other.capacity_ = 0;
        other.data_ = nullptr;
    }

    Tensor& operator=(Tensor&& other) noexcept
    {
        if (this == &other) {
            return *this;
        }

        allocator::deallocate(data_);
        size_ = other.size_;
        capacity_ = other.capacity_;
        data_ = other.data_;

        other.size_ = 0;
        other.capacity_ = 0;
        other.data_ = nullptr;

        return *this;
    }

    Tensor(std::initializer_list<value_type> list) :
        Tensor(list.begin(), list.end())
    { }

    template<typename U>
    Tensor<U> cast() const
    {
        Tensor<U> res;
        res.reserve(size_);
        for (size_type i = 0; i < size_; ++i) {
            res.push_back(data_[i]);
        }
        return res;
    }

    static inline Tensor from_literal(std::string_view str, bool* error)
    {
        *error = false;

        if (str.empty()) {
            // empty string
            *error = true;
            return {};
        }

        if (str.front() != '[' || str.back() != ']') {
            // missing brackets
            *error = true;
            return {};
        }

        if (str.size() == 2) {
            // empty tensor
            return {};
        }

        Tensor tensor;

        bool EXPECT_COMMA = false;
        std::size_t i = 1;
        while (i < str.size() - 1) {
            // skip whitespaces
            while (std::isspace(str[i])) {
                ++i;
            }

            if (i == str.size() - 1) {
                // reached the end
                break;
            }

            // verify separator/end
            if (EXPECT_COMMA) {
                if (str[i] == ',') {
                    ++i;
                    EXPECT_COMMA = false;
                    continue;
                } else {
                    *error = true;
                    return {};
                }
            }

            // Parse values
            try {
                std::size_t bytes_read = 0;
                if constexpr (std::is_same_v<T, float>) {
                    tensor.push_back(std::stof(&str[i], &bytes_read));
                } else if constexpr (std::is_same_v<T, double>) {
                    tensor.push_back(std::stod(&str[i], &bytes_read));
                }
                i += bytes_read;
                EXPECT_COMMA = true;
            } catch (...) {
                *error = true;
                return {};
            }
        }

        return tensor;
    }

    static inline Tensor from_bytes(const char* bytes, size_type size)
    {
        assert(size % sizeof(value_type) == 0 && "this should never happen");
        return Tensor(bytes, size);
    }

    ~Tensor() noexcept
    {
        clear();
    }

    // ============================================================================
    // Accessors
    // ============================================================================
    size_type size() const
    {
        return size_;
    }

    size_type capacity() const
    {
        return capacity_;
    }

    bool empty() const
    {
        return size_ == 0;
    }

    pointer data()
    {
        return data_;
    }
    const_pointer data() const
    {
        return data_;
    }

    reference operator[](size_type i)
    {
        assert(i < size_);
        return data_[i];
    }
    const_reference operator[](size_type i) const
    {
        assert(i < size_);
        return data_[i];
    }

    // ============================================================================
    // Allocation
    // ============================================================================
    void clear()
    {
        if (data_) {
            allocator::deallocate(data_);
            data_ = nullptr;
            size_ = 0;
            capacity_ = 0;
        }
    }

    void reserve(size_type n)
    {
        if (n <= capacity_) {
            return;
        }

        n = round_up_allocation_size(n);
        pointer new_data = allocator::allocate(n);

        if (data_) {
            std::memcpy(new_data, data_, sizeof(value_type) * size_);
            allocator::deallocate(data_);
        }

        data_ = new_data;
        capacity_ = n;
    }

    void resize(size_type n)
    {
        if (n > capacity_) {
            reserve(n);
        }

        size_ = n;
    }

    void push_back(value_type val)
    {
        if (size_ == capacity_) {
            reserve(capacity_ == 0 ? 1 : 2 * capacity_);
        }

        data_[size_++] = val;
    }

    // ============================================================================
    // Comparisons
    // ============================================================================
    static inline int compare(const Tensor& lhs, const Tensor& rhs)
    {
        return operations::comparison<value_type>::cmp(lhs.data_, rhs.data_, lhs.size_, rhs.size_);
    }

    bool operator==(const Tensor& rhs) const
    {
        return operations::comparison<value_type>::eq(data_, rhs.data_, size_, rhs.size_);
    }

    bool operator!=(const Tensor& rhs) const
    {
        return operations::comparison<value_type>::neq(data_, rhs.data_, size_, rhs.size_);
    }

    bool operator<(const Tensor& rhs) const
    {
        return operations::comparison<value_type>::lt(data_, rhs.data_, size_, rhs.size_);
    }

    bool operator>(const Tensor& rhs) const
    {
        return operations::comparison<value_type>::gt(data_, rhs.data_, size_, rhs.size_);
    }

    bool operator<=(const Tensor& rhs) const
    {
        return operations::comparison<value_type>::leq(data_, rhs.data_, size_, rhs.size_);
    }

    bool operator>=(const Tensor& rhs) const
    {
        return operations::comparison<value_type>::geq(data_, rhs.data_, size_, rhs.size_);
    }

    // ============================================================================
    // Modifiers
    // ============================================================================
    void set(T val)
    {
        operations::modifiers<value_type>::set(data_, val, size_);
    }

    void negate()
    {
        operations::modifiers<value_type>::neg(data_, size_);
    }

    void abs()
    {
        operations::modifiers<value_type>::abs(data_, size_);
    }

    void ceil()
    {
        operations::modifiers<value_type>::ceil(data_, size_);
    }

    void floor()
    {
        operations::modifiers<value_type>::floor(data_, size_);
    }

    // Special case where SPARQL-1.1 ROUND != std::round
    // SPARQL ROUND(-0.5f) == 0
    // std::round(-0.5f) == -1
    void sparql11_round()
    {
        operations::modifiers<value_type>::sparql11_round(data_, size_);
    }

    void inverse()
    {
        operations::modifiers<value_type>::inverse(data_, size_);
    }

    void sqrt()
    {
        operations::modifiers<value_type>::sqrt(data_, size_);
    }

    // ============================================================================
    // Vectorial arithmetic
    // ============================================================================
    // this += rhs
    void inplace_add_tensor(const Tensor& rhs) {
        assert(size_ == rhs.size_);

        operations::vectorial_arithmetic<value_type>::add(data_, rhs.data_, data_, size_);
    }

    // this -= rhs
    void inplace_sub_tensor(const Tensor& rhs)
    {
        assert(size_ == rhs.size_);

        operations::vectorial_arithmetic<value_type>::sub(data_, rhs.data_, data_, size_);
    }

    // this *= rhs
    void inplace_mul_tensor(const Tensor& rhs)
    {
        assert(size_ == rhs.size_);

        operations::vectorial_arithmetic<value_type>::mul(data_, rhs.data_, data_, size_);
    }

    // this /= rhs
    void inplace_div_tensor(const Tensor& rhs)
    {
        assert(size_ == rhs.size_);

        operations::vectorial_arithmetic<value_type>::div(data_, rhs.data_, data_, size_);
    }

    // ============================================================================
    // Scalar arithmetic
    // ============================================================================
    // this[i] += rhs
    void inplace_add_scalar(T rhs)
    {
        operations::scalar_arithmetic<value_type>::add(data_, rhs, data_, size_);
    }

    // this[i] -= rhs
    void inplace_sub_scalar_rhs(T rhs)
    {
        operations::scalar_arithmetic<value_type>::sub(data_, rhs, data_, size_);
    }

    // this[i] = lhs - this[i]
    void inplace_sub_scalar_lhs(T lhs)
    {
        operations::scalar_arithmetic<value_type>::sub_lhs(lhs, data_, data_, size_);
    }

    // this[i] *= rhs
    void inplace_mul_scalar(T rhs)
    {
        operations::scalar_arithmetic<value_type>::mul(data_, rhs, data_, size_);
    }

    // this[i] /= rhs
    void inplace_div_scalar_rhs(T rhs)
    {
        operations::scalar_arithmetic<value_type>::div(data_, rhs, data_, size_);
    }

    // this[i] = lhs / this[i]
    void inplace_div_scalar_lhs(T lhs)
    {
        operations::scalar_arithmetic<value_type>::div_lhs(lhs, data_, data_, size_);
    }

    void pow(T rhs)
    {
        operations::scalar_arithmetic<value_type>::pow(data_, rhs, data_, size_);
    }

    // ============================================================================
    // Computation
    // ============================================================================
    value_type sum() const
    {
        return operations::computation<value_type>::sum(data_, size_);
    }

    static inline value_type sum(const Tensor& tensor)
    {
        return tensor.sum();
    }

    value_type euclidean_distance(const Tensor& rhs) const
    {
        assert(size_ == rhs.size_);

        return operations::computation<value_type>::euclidean_distance(data_, rhs.data_, size_);
    }

    static inline value_type euclidean_distance(const Tensor& lhs, const Tensor& rhs)
    {
        return lhs.euclidean_distance(rhs);
    }

    value_type manhattan_distance(const Tensor& rhs) const
    {
        assert(size_ == rhs.size_);

        return operations::computation<value_type>::manhattan_distance(data_, rhs.data_, size_);
    }

    static inline value_type manhattan_distance(const Tensor& lhs, const Tensor& rhs)
    {
        return lhs.manhattan_distance(rhs);
    }

    value_type cosine_similarity(const Tensor& rhs) const
    {
        assert(size_ == rhs.size_);

        return operations::computation<value_type>::cosine_similarity(data_, rhs.data_, size_);
    }

    static inline value_type cosine_similarity(const Tensor& lhs, const Tensor& rhs)
    {
        return lhs.cosine_similarity(rhs);
    }

    value_type cosine_distance(const Tensor& rhs) const
    {
        assert(size_ == rhs.size_);

        return operations::computation<value_type>::cosine_distance(data_, rhs.data_, size_);
    }

    static inline value_type cosine_distance(const Tensor& lhs, const Tensor& rhs)
    {
        return lhs.cosine_distance(rhs);
    }

    value_type dot(const Tensor& rhs) const
    {
        assert(size_ == rhs.size_);

        return operations::computation<value_type>::dot(data_, rhs.data_, size_);
    }

    static inline value_type dot(const Tensor& lhs, const Tensor& rhs)
    {
        return lhs.dot(rhs);
    }

    // ============================================================================
    // Misc
    // ============================================================================
    static constexpr uint64_t get_inline_mask()
    {
        if constexpr (std::is_same_v<value_type, float>) {
            return ObjectId::MASK_TENSOR_FLOAT_INLINED;
        } else if constexpr (std::is_same_v<value_type, double>) {
            return ObjectId::MASK_TENSOR_DOUBLE_INLINED;
        }
    }

    static constexpr uint64_t get_external_mask()
    {
        if constexpr (std::is_same_v<value_type, float>) {
            return ObjectId::MASK_TENSOR_FLOAT_EXTERN;
        } else if constexpr (std::is_same_v<value_type, double>) {
            return ObjectId::MASK_TENSOR_DOUBLE_EXTERN;
        }
    }

    static constexpr uint64_t get_tmp_mask()
    {
        if constexpr (std::is_same_v<value_type, float>) {
            return ObjectId::MASK_TENSOR_FLOAT_TMP;
        } else if constexpr (std::is_same_v<value_type, double>) {
            return ObjectId::MASK_TENSOR_DOUBLE_TMP;
        }
    }

    static constexpr uint64_t get_subtype() {
        if constexpr (std::is_same_v<value_type, float>) {
            return ObjectId::MASK_TENSOR_FLOAT;
        } else if constexpr (std::is_same_v<value_type, double>) {
            return ObjectId::MASK_TENSOR_DOUBLE;
        }
    }

    std::string to_string() const
    {
        if (size_ == 0) {
            return "[]";
        }

        std::string res = "[" + std::to_string(data_[0]);
        for (size_type i = 0; i < size_; ++i) {
            res += ", ";
            res += std::to_string(data_[i]);
        }
        return res + "]";
    }

    friend std::ostream& operator<<(std::ostream& os, const Tensor& tensor)
    {
        return os << tensor.to_string();
    }

private:
    size_type size_;
    size_type capacity_;
    pointer data_;

    static constexpr size_type round_up_allocation_size(size_type n)
    {
        const auto rounded = (sizeof(value_type) * n + operations::alignment_bytes - 1)
                           & ~(operations::alignment_bytes - 1);
        return rounded / sizeof(value_type);
    }
};

} // namespace tensor
