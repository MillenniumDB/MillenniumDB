#include "disk_int_stack.h"

#include <unistd.h>

// #include "misc/logger.h"
#include "system/buffer_manager.h"

// template<typename T>
// void DiskIntStack<T>::create(FileId file_id)
// {
//     const T stack_size(0);
//     const auto write_res = pwrite(file_id.id, &stack_size, sizeof(T), 0);
//     if (write_res == -1) {
//         throw std::runtime_error("Could not write into DiskIntStack file");
//     }
// }

template<typename T>
DiskIntStack<T>::DiskIntStack(FileId file_id) :
    file_id(file_id)
{
    first_page = &buffer_manager.get_page_readonly(file_id, 0);

    auto stack_size = size();

    auto end = stack_size * sizeof(T) + sizeof(uint64_t);

    // TODO: init last_page
}

template<typename T>
DiskIntStack<T>::~DiskIntStack()
{
    buffer_manager.unpin(*first_page);
    buffer_manager.unpin(*last_page);
}

template<typename T>
void DiskIntStack<T>::push(T value)
{
    // upgrade to editable first page to increment size
    if (buffer_manager.need_edit_version(*first_page)) {
        auto new_page = &buffer_manager.get_page_editable(file_id, first_page->get_page_number());
        buffer_manager.unpin(*first_page);
        first_page = new_page;
    }
    auto* stack_size = reinterpret_cast<uint64_t*>(first_page->get_bytes());
    *stack_size = *stack_size + 1;

    // TODO: decide if insert in last_page or create another one

}

template<typename T>
T DiskIntStack<T>::pop()
{
    if (empty()) {
        return 0;
    }
    // upgrade to editable first page to decrement size
    if (buffer_manager.need_edit_version(*first_page)) {
        auto new_page = &buffer_manager.get_page_editable(file_id, first_page->get_page_number());
        buffer_manager.unpin(*first_page);
        first_page = new_page;
    }
    auto* stack_size = reinterpret_cast<uint64_t*>(first_page->get_bytes());
    *stack_size = *stack_size - 1;

    // TODO: read value at new stack_size and return it
}

template<typename T>
uint64_t DiskIntStack<T>::size() const noexcept {
    return *reinterpret_cast<uint64_t*>(first_page->get_bytes());
}

template class DiskIntStack<uint32_t>;
template class DiskIntStack<uint64_t>;
