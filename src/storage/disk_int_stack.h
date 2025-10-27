#pragma once

#include <optional>
#include <type_traits>

#include "storage/file_id.h"

/**
On disk integral stack.
The first 8 bytes of the file have the size of the stack
then we have the array of integers
 */
template<typename T>
class DiskIntStack {
    static_assert(std::is_integral<T>::value, "T must be an integral type");

public:
    DiskIntStack(FileId file_id);

    void push(T value);

    std::optional<T> try_pop();

    // uint64_t size() const noexcept;

    // bool empty() const noexcept
    // {
    //     return size() == 0;
    // }

private:
    FileId file_id;
};
