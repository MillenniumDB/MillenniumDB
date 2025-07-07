#pragma once

#include <cassert>
#include <cstring>
#include <memory>
#include <vector>

template<typename T, typename Compare = std::less<T>>
class MinMaxHeap {
public:
    using value_type = T;
    using size_type = std::size_t;

    static constexpr size_type DEFAULT_INITIAL_SIZE = 8;
    static constexpr size_type GROWTH_FACTOR = 2;

    MinMaxHeap() :
        MinMaxHeap(DEFAULT_INITIAL_SIZE) {};

    // construct a MinMaxHeap with an initial capacity
    explicit MinMaxHeap(size_type initial_capacity) :
        size_ { 0 },
        capacity_ { initial_capacity },
        heap { std::make_unique<T[]>(capacity_) }
    { }

    // extract and return an ascending vector of the minimum values
    std::vector<value_type> extract_n_min(size_type n)
    {
        n = std::min(size_, n);
        std::vector<value_type> res;
        res.reserve(n);
        while (res.size() < n) {
            res.emplace_back(get_min());
            pop_min();
        }
        return res;
    }

    // extract and return a descending vector of the maximum values
    std::vector<T> extract_n_max(size_type n)
    {
        n = std::min(size_, n);
        std::vector<T> res;
        res.reserve(n);
        while (res.size() < n) {
            res.emplace_back(get_max());
            pop_max();
        }
        return res;
    }

    // get a const reference to the minimum
    const T& get_min() const
    {
        assert(!empty());
        return heap[0];
    }

    // get a reference to the minimum
    T& get_min()
    {
        assert(!empty());
        return heap[0];
    }

    // get const a reference to the maximum
    const T& get_max() const
    {
        assert(!empty());
        switch (size_) {
        case 1:
            return heap[0];
        case 2:
            return heap[1];
        default:
            return compare(heap[1], heap[2]) ? heap[2] : heap[1];
        }
    }

    // get a reference to the maximum
    T& get_max()
    {
        assert(!empty());
        switch (size_) {
        case 1:
            return heap[0];
        case 2:
            return heap[1];
        default:
            return compare(heap[1], heap[2]) ? heap[2] : heap[1];
        }
    }

    template<typename... Args>
    void emplace(Args&&... args)
    {
        ensure_capacity();
        heap[size_] = T(std::forward<Args>(args)...);
        push_up(size_);
        ++size_;
    }

    void push(const T& entry)
    {
        emplace(entry);
    }

    void push(T&& entry)
    {
        emplace(std::move(entry));
    }

    void pop_min()
    {
        assert(!empty());
        heap[0] = std::move(heap[size_ - 1]);
        --size_;
        if (size_ > 0) {
            push_down(0);
        }
    }

    void pop_max()
    {
        assert(!empty());

        if (size_ == 1) {
            --size_;
            return;
        }

        const size_type max_pos = (size_ > 2 && compare(heap[1], heap[2])) ? 2 : 1;
        heap[max_pos] = std::move(heap[size_ - 1]);
        --size_;

        push_down(max_pos);
    }

    inline void clear()
    {
        size_ = 0;
    }

    inline size_type size() const
    {
        return size_;
    }

    inline size_type capacity() const
    {
        return capacity_;
    }

    inline bool empty() const
    {
        return size_ == 0;
    }

private:
    size_type size_;
    size_type capacity_;
    Compare compare;
    std::unique_ptr<T[]> heap;

    static inline bool is_min_level(size_type i)
    {
        int level = 0;
        while (i > 0) {
            i = parent(i);
            ++level;
        }
        return level % 2 == 0;
    }

    static inline size_type parent(size_type i)
    {
        return (i - 1) / 2;
    }

    static inline size_type grandparent(size_type i)
    {
        return parent(parent(i));
    }

    static inline size_type left_child(size_type i)
    {
        return 2 * i + 1;
    }

    static inline size_type right_child(size_type i)
    {
        return 2 * i + 2;
    }

    inline bool has_children(size_type i) const
    {
        return left_child(i) < size_;
    }

    inline bool has_grandchildren(size_type i) const
    {
        return left_child(left_child(i)) < size_;
    }

    inline void ensure_capacity()
    {
        // static_assert(std::is_trivially_copyable<T>::value);

        if (size_ == capacity_) {
            const size_type new_capacity = GROWTH_FACTOR * capacity_;
            auto new_heap = std::make_unique<T[]>(new_capacity);
            std::memcpy(new_heap.get(), heap.get(), sizeof(T) * size_);
            heap = std::move(new_heap);
            capacity_ = new_capacity;
        }
    }

    inline void push_up(size_type i)
    {
        if (i == 0) {
            return;
        }

        if (is_min_level(i)) {
            if (compare(heap[parent(i)], heap[i])) {
                std::swap(heap[parent(i)], heap[i]);
                push_up_max(parent(i));
            } else {
                push_up_min(i);
            }
        } else {
            if (compare(heap[i], heap[parent(i)])) {
                std::swap(heap[i], heap[parent(i)]);
                push_up_min(parent(i));
            } else {
                push_up_max(i);
            }
        }
    }

    inline void push_up_min(size_t i)
    {
        if (i > 2) {
            const size_type gp_pos = grandparent(i);
            if (compare(heap[i], heap[gp_pos])) {
                std::swap(heap[i], heap[gp_pos]);
                push_up_min(gp_pos);
            }
        }
    }

    inline void push_up_max(size_t i)
    {
        if (i > 2) {
            const size_type gp_pos = grandparent(i);
            if (compare(heap[gp_pos], heap[i])) {
                std::swap(heap[gp_pos], heap[i]);
                push_up_max(gp_pos);
            }
        }
    }

    inline void push_down(size_type i)
    {
        if (is_min_level(i)) {
            push_down_min(i);
        } else {
            push_down_max(i);
        }
    }

    inline void push_down_min(size_type i)
    {
        while (has_children(i)) {
            const size_type l_pos = left_child(i);
            const size_type r_pos = right_child(i);
            size_type m_pos = i;

            if (l_pos < size_ && compare(heap[l_pos], heap[m_pos])) {
                m_pos = l_pos;
            }
            if (r_pos < size_ && compare(heap[r_pos], heap[m_pos])) {
                m_pos = r_pos;
            }

            if (has_grandchildren(i)) {
                const size_type ll_pos = left_child(l_pos);
                const size_type lr_pos = right_child(l_pos);
                const size_type rl_pos = left_child(r_pos);
                const size_type rr_pos = right_child(r_pos);

                if (ll_pos < size_ && compare(heap[ll_pos], heap[m_pos])) {
                    m_pos = ll_pos;
                }
                if (lr_pos < size_ && compare(heap[lr_pos], heap[m_pos])) {
                    m_pos = lr_pos;
                }
                if (rl_pos < size_ && compare(heap[rl_pos], heap[m_pos])) {
                    m_pos = rl_pos;
                }
                if (rr_pos < size_ && compare(heap[rr_pos], heap[m_pos])) {
                    m_pos = rr_pos;
                }
            }

            if (m_pos == i)
                break;

            std::swap(heap[i], heap[m_pos]);
            if (m_pos >= left_child(left_child(i))) {
                const size_type m_parent_pos = parent(m_pos);
                if (compare(heap[m_parent_pos], heap[m_pos])) {
                    std::swap(heap[m_parent_pos], heap[m_pos]);
                }
                i = m_pos;
            } else {
                break;
            }
        }
    }

    inline void push_down_max(size_type i)
    {
        while (has_children(i)) {
            const size_type l_pos = left_child(i);
            const size_type r_pos = right_child(i);
            size_type m_pos = i;

            if (l_pos < size_ && compare(heap[m_pos], heap[l_pos])) {
                m_pos = l_pos;
            }
            if (r_pos < size_ && compare(heap[m_pos], heap[r_pos])) {
                m_pos = r_pos;
            }

            if (has_grandchildren(i)) {
                const size_type ll_pos = left_child(l_pos);
                const size_type lr_pos = right_child(l_pos);
                const size_type rl_pos = left_child(r_pos);
                const size_type rr_pos = right_child(r_pos);

                if (ll_pos < size_ && compare(heap[m_pos], heap[ll_pos])) {
                    m_pos = ll_pos;
                }
                if (lr_pos < size_ && compare(heap[m_pos], heap[lr_pos])) {
                    m_pos = lr_pos;
                }
                if (rl_pos < size_ && compare(heap[m_pos], heap[rl_pos])) {
                    m_pos = rl_pos;
                }
                if (rr_pos < size_ && compare(heap[m_pos], heap[rr_pos])) {
                    m_pos = rr_pos;
                }
            }

            if (m_pos == i)
                break;

            std::swap(heap[i], heap[m_pos]);
            if (m_pos >= left_child(left_child(i))) {
                const size_type m_parent_pos = parent(m_pos);
                if (compare(heap[m_pos], heap[m_parent_pos])) {
                    std::swap(heap[m_pos], heap[m_parent_pos]);
                }
                i = m_pos;
            } else {
                break;
            }
        }
    }
};
