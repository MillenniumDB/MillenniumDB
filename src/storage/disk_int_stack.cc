#include "disk_int_stack.h"

#include <unistd.h>

#include "misc/logger.h"

template<typename T>
void DiskIntStack<T>::create(FileId file_id)
{
    const T stack_size(0);
    const auto write_res = pwrite(file_id.id, &stack_size, sizeof(T), 0);
    if (write_res == -1) {
        throw std::runtime_error("Could not write into DiskIntStack file");
    }
}

template<typename T>
DiskIntStack<T>::DiskIntStack(FileId file_id_) :
    file_id { file_id_ }
{
    T stack_size;
    const auto read_res = pread(file_id.id, &stack_size, sizeof(T), 0);
    if (read_res == -1) {
        throw std::runtime_error("Could not read from DiskIntStack file");
    }

    if (stack_size > 0) {
        stack.resize(stack_size);
        const auto read_res2 = pread(file_id.id, &stack[0], sizeof(T) * stack_size, sizeof(T));
        if (read_res2 == -1) {
            throw std::runtime_error("Could not read from DiskIntStack file");
        }
    }
}

template<typename T>
DiskIntStack<T>::~DiskIntStack()
{
    if (modified) {
        const auto stack_size = stack.size();
        auto write_res = pwrite(file_id.id, &stack_size, sizeof(T), 0);
        if (write_res == -1) {
            logger(Category::Error) << "Could not write into DiskIntStack header";
        }

        write_res = pwrite(file_id.id, &stack[0], sizeof(T) * stack.size(), sizeof(T));
        if (write_res == -1) {
            logger(Category::Error) << "Could not write into DiskIntStack stack";
        }
    }
}

template<typename T>
void DiskIntStack<T>::push(T value)
{
    stack.emplace_back(value);
    modified = true;
}

template<typename T>
T DiskIntStack<T>::pop()
{
    const auto value = stack.back();
    stack.pop_back();
    modified = true;
    return value;
}

template class DiskIntStack<uint32_t>;
template class DiskIntStack<uint64_t>;