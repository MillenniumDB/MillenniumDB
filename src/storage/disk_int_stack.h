#pragma once

#include <type_traits>

#include "storage/file_id.h"
#include "storage/page/versioned_page.h"

/**
 * On disk integral stack.
 * File structure:
 * - T num_elements (uint64)
 * - T stack[num_elements]
 */
template<typename T>
class DiskIntStack {
    static_assert(std::is_integral<T>::value, "T must be an integral type");

public:
    DiskIntStack(FileId file_id);

    ~DiskIntStack();

    void push(T value);

    T pop();

    uint64_t size() const noexcept;

    bool empty() const noexcept
    {
        return size() == 0;
    }

private:
    FileId file_id;

    // Never nullptr
    Page* first_page;

    // Never nullptr, may be the same first page
    Page* last_page;

    // bool modified = false;

    // std::vector<T> stack;
};
