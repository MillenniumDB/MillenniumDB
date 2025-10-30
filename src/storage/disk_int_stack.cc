#include "disk_int_stack.h"

#include <unistd.h>

#include "system/buffer_manager.h"

template<typename T>
DiskIntStack<T>::DiskIntStack(FileId file_id) :
    file_id(file_id)
{ }

template<typename T>
void DiskIntStack<T>::push(T value)
{
    auto& first_page = buffer_manager.get_page_editable(file_id, 0);

    auto* stack_size = reinterpret_cast<uint64_t*>(first_page.get_bytes());
    *stack_size = *stack_size + 1;

    auto write_total_offset = *stack_size * sizeof(T);
    auto write_page_number = write_total_offset / Page::SIZE;
    auto offset_in_page = write_total_offset % Page::SIZE;

    auto& last_page = buffer_manager.get_page_editable(file_id, write_page_number);

    auto* write_ptr = reinterpret_cast<T*>(last_page.get_bytes() + offset_in_page);
    *write_ptr = value;

    buffer_manager.unpin(first_page);
    buffer_manager.unpin(last_page);
}

template<typename T>
std::optional<T> DiskIntStack<T>::try_pop()
{
    auto& first_page = buffer_manager.get_page_editable(file_id, 0);
    auto* stack_size = reinterpret_cast<uint64_t*>(first_page.get_bytes());

    if (*stack_size == 0) {
        buffer_manager.unpin(first_page);
        return {};
    }

    *stack_size = *stack_size - 1;

    auto write_total_offset = *stack_size * sizeof(T);
    auto write_page_number = write_total_offset / Page::SIZE;
    auto offset_in_page = write_total_offset % Page::SIZE;

    auto& last_page = buffer_manager.get_page_editable(file_id, write_page_number);

    auto res = *reinterpret_cast<T*>(last_page.get_bytes() + offset_in_page);

    buffer_manager.unpin(first_page);
    buffer_manager.unpin(last_page);

    return std::optional<T>(res);
}

template class DiskIntStack<uint32_t>;
template class DiskIntStack<uint64_t>;
