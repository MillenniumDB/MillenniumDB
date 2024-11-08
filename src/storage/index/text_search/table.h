#pragma once

#include <filesystem>

#include "storage/page/unversioned_page.h"

namespace TextSearch {

class Table {
public:
    // Creates a new table or loads it if it already exists
    Table(std::filesystem::path path, uint64_t column_count);

    ~Table();

    // Inserts a row into the table and return the table pointer to the inserted row
    uint64_t insert(std::vector<uint64_t> values);

    // Obtains the row pointed to by page_pointer
    std::vector<uint64_t> get(uint64_t page_pointer);

private:
    // FileId of the file containing the table
    const FileId file_id;
    // The first page of the table, used to save metadata
    UPage& first_page;
    // The currently loaded page, to avoid having to obtain the page
    // for every consecutive insert
    UPage* current_page = nullptr;

    // the number of columns in the table.
    // 8 first bytes of the page, not using a pointer as this won't change
    const uint64_t column_count;

    // page pointer (page number + page offset) to the first unused byte
    // 8 Bytes, placed after column_count
    unsigned char* end_page_pointer;

    static constexpr size_t COLUMN_COUNT_SIZE = 8;
    static constexpr size_t PAGE_POINTER_SIZE = 8;
    static constexpr size_t HEADER_SIZE = COLUMN_COUNT_SIZE + PAGE_POINTER_SIZE;
};

} // namespace TextSearch
