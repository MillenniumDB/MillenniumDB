#pragma once

#include <memory>
#include <type_traits>
#include <vector>

#include "storage/file_id.h"

/**
 * On disk integral stack. As we support one update at a time and this
 * structure is update-only, we do not need to versionate it. It is
 * assumed that the stack size will be small enough to fit in memory
 *
 * File structure:
 * - T num_elements
 * - T stack[num_elements]
 */
template<typename T>
class DiskIntStack {
    static_assert(std::is_integral<T>::value, "T must be an integral type");

public:
    static void create(FileId file_id);

    explicit DiskIntStack(FileId file_id);

    ~DiskIntStack();

    void push(T value);

    T pop();

    inline bool empty() const noexcept
    {
        return stack.empty();
    }

    inline std::size_t size() const noexcept
    {
        return stack.size();
    }

private:
    FileId file_id;

    bool modified { false };

    std::vector<T> stack;
};