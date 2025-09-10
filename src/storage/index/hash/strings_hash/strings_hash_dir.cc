#include "strings_hash_dir.h"

#include "system/buffer_manager.h"

StringsHashDir::StringsHashDir(FileId file_id) :
    file_id(file_id)
{
    first_page = &buffer_manager.get_page_readonly(file_id, 0);
}

StringsHashDir::~StringsHashDir()
{
    buffer_manager.unpin(*first_page);
}

void StringsHashDir::upgrade_to_editable()
{
    if (!buffer_manager.is_editable(*first_page)) {
        auto new_page = &buffer_manager.get_page_editable(file_id, 0);
        buffer_manager.unpin(*first_page);
        first_page = new_page;
    }
}

// increments total pages
uint32_t StringsHashDir::get_new_bucket_number()
{
    upgrade_to_editable();
    uint32_t* total_pages = reinterpret_cast<uint32_t*>(first_page->get_bytes() + sizeof(uint32_t));
    auto res = *total_pages;

    *total_pages = res + 1;
    return res;
}

uint32_t StringsHashDir::global_depth() const
{
    return *reinterpret_cast<uint32_t*>(first_page->get_bytes());
}

uint32_t StringsHashDir::get_bucket_number(uint32_t pos)
{
    auto file_offset = 8 + pos * sizeof(uint32_t);
    auto page_number = file_offset / Page::SIZE;
    auto page_offset = file_offset % Page::SIZE;

    auto& page = buffer_manager.get_page_readonly(file_id, page_number);
    auto res = *reinterpret_cast<uint32_t*>(page.get_bytes() + page_offset);
    buffer_manager.unpin(page);
    return res;
}

void StringsHashDir::set_bucket_number(uint32_t pos, uint32_t bucket_number)
{
    auto file_offset = 8 + pos * sizeof(uint32_t);
    auto page_number = file_offset / Page::SIZE;
    auto page_offset = file_offset % Page::SIZE;

    auto& page = buffer_manager.get_page_editable(file_id, page_number);
    *reinterpret_cast<uint32_t*>(page.get_bytes() + page_offset) = bucket_number;
    buffer_manager.unpin(page);
}

void StringsHashDir::duplicate()
{
    upgrade_to_editable();
    auto global_depth_ptr = reinterpret_cast<uint32_t*>(first_page->get_bytes());
    auto g_depth = global_depth();

    auto old_dir_size = 1ULL << g_depth;
    *global_depth_ptr = g_depth + 1;
    auto new_dir_size = 2 * old_dir_size;

    for (auto i = old_dir_size; i < new_dir_size; i++) {
        // TODO: may be inefficient in terms of getting pages
        set_bucket_number(i, get_bucket_number(i - old_dir_size));
    }
}
