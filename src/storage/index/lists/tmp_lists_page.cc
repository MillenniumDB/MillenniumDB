#include "tmp_lists_page.h"

#include "system/buffer_manager.h"

TmpListsPage::TmpListsPage(TmpFileId file_id, uint_fast32_t list_page_number, uint64_t offset) :
    insert_offset(offset),
    page(buffer_manager.get_ppage(file_id, list_page_number)),
    page_start(page.get_bytes())
{ }

TmpListsPage::~TmpListsPage()
{
    buffer_manager.unpin(page);
}

uint64_t TmpListsPage::insert(const std::vector<ObjectId>& list, uint64_t* bytes_left_next_page)
{
    page.make_dirty();
    uint64_t initial_offset = insert_offset;
    // +1 because we store the size in 1 byte
    insert(list, list.size() * sizeof(uint64_t) + 1, bytes_left_next_page);
    return initial_offset;
}

void TmpListsPage::insert(
    const std::vector<ObjectId>& list,
    uint64_t bytes_left_this_page,
    uint64_t* bytes_left_next_page
)
{
    uint64_t initial_offset = insert_offset;
    char* current_pos = page_start + insert_offset;
    uint64_t bytes_left_in_list = bytes_left_this_page;

    // write the list size
    if (bytes_left_this_page > list.size() * OID_SIZE) {
        if (insert_offset + sizeof(uint8_t) <= PPage::SIZE) {
            uint8_t list_size = list.size();
            memcpy(current_pos, &list_size, sizeof(uint8_t));
            current_pos += sizeof(uint8_t);
            insert_offset += sizeof(uint8_t);
            // used to calculate the initial pos and the bytes of the first item
            bytes_left_in_list -= sizeof(uint8_t);
        } else {
            *bytes_left_next_page = list.size() * OID_SIZE + 1;
            return;
        }
    }
    if (list.size() == 0) {
        *bytes_left_next_page = 0;
        return;
    }

    uint64_t total_size = list.size() * OID_SIZE;
    uint64_t initial_pos = (total_size - (bytes_left_in_list)) / OID_SIZE;
    uint64_t bytes_item = OID_SIZE - (total_size - bytes_left_in_list) % OID_SIZE;

    // write the first item
    if (insert_offset + bytes_item <= PPage::SIZE) {
        char* item = (char*) (&(list[initial_pos]));
        memcpy(current_pos, item + OID_SIZE - bytes_item, bytes_item);
        current_pos += bytes_item;
        insert_offset += bytes_item;
    } else {
        bytes_item = PPage::SIZE - insert_offset;
        char* item = (char*) (&(list[initial_pos]));
        memcpy(current_pos, item, bytes_item);
        current_pos += bytes_item;
        insert_offset += bytes_item;
        *bytes_left_next_page = (bytes_left_this_page - (insert_offset - initial_offset));
        return;
    }

    // write the the rest of the items
    uint64_t i = initial_pos + 1;
    for (; i < list.size(); ++i) {
        if (insert_offset + OID_SIZE <= PPage::SIZE) {
            memcpy(current_pos, &(list[i]), OID_SIZE);
            current_pos += OID_SIZE;
            insert_offset += OID_SIZE;
        } else {
            bytes_item = PPage::SIZE - insert_offset;
            memcpy(current_pos, &(list[i]), bytes_item);
            insert_offset += bytes_item;
            break;
        }
    }

    *bytes_left_next_page = (bytes_left_this_page - (insert_offset - initial_offset));
}

uint64_t TmpListsPage::get_list_size(uint64_t offset)
{
    return *(page_start + offset);
}

uint64_t
    TmpListsPage::get(uint64_t page_offset, char* buffer, uint64_t buffer_pos, uint64_t* bytes_left_next_page)
{
    uint64_t list_size = *(page_start + page_offset) * OID_SIZE;
    return get(page_offset + 1, buffer, buffer_pos, list_size, bytes_left_next_page);
}

uint64_t TmpListsPage::get(
    uint64_t page_offset,
    char* buffer,
    uint64_t buffer_pos,
    uint64_t bytes_left_this_page,
    uint64_t* bytes_left_next_page
)
{
    if (bytes_left_this_page > PPage::SIZE - page_offset) {
        memcpy(buffer + buffer_pos, page_start + page_offset, PPage::SIZE - page_offset);
        *bytes_left_next_page = bytes_left_this_page - (PPage::SIZE - page_offset);
    } else {
        memcpy(buffer + buffer_pos, page_start + page_offset, bytes_left_this_page);
        *bytes_left_next_page = 0;
    }

    return bytes_left_this_page - *bytes_left_next_page;
}
