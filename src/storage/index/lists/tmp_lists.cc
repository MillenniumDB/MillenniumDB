#include "tmp_lists.h"

#include "storage/index/lists/tmp_lists_page.h"
#include "system/buffer_manager.h"

TmpLists::TmpLists() :
    file_id(buffer_manager.get_tmp_file_id())
{
    // +1 because it stores the size of the list in 1 byte
    buffer = new char[MAX_LIST_SIZE + 1];
}

TmpLists::~TmpLists()
{
    buffer_manager.remove_tmp(file_id);
    delete[] buffer;
}

uint32_t TmpLists::get_file_id()
{
    return file_id.id;
}

uint64_t TmpLists::insert(const std::vector<ObjectId>& list)
{
    TmpListsPage list_page(file_id, current_page);
    uint64_t initial_page = current_page;

    uint64_t remaining_bytes;
    uint64_t page_offset = list_page.insert(list, &remaining_bytes);

    while (remaining_bytes) {
        current_page = ++total_pages;
        TmpListsPage list_page(file_id, current_page);
        list_page.insert(list, remaining_bytes, &remaining_bytes);
    }

    return PPage::SIZE * initial_page + page_offset;
}

void TmpLists::get(std::vector<ObjectId>& out, uint64_t offset)
{
    uint64_t page = offset / PPage::SIZE;

    TmpListsPage list_page(file_id, page);

    uint64_t remaining_bytes;
    uint64_t buffer_pos = list_page.get(offset, buffer, 0, &remaining_bytes);

    while (remaining_bytes) {
        TmpListsPage list_page(file_id, ++page);

        buffer_pos += list_page.get(0, buffer, buffer_pos, remaining_bytes, &remaining_bytes);
    }

    uint64_t list_size = list_page.get_list_size(offset);

    for (uint64_t i = 0; i < list_size; ++i) {
        ObjectId* oid = reinterpret_cast<ObjectId*>(buffer + i * sizeof(ObjectId));
        out.push_back(*oid);
    }
}
