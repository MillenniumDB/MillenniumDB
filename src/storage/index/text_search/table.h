#pragma once

#include <filesystem>

#include "storage/disk_int_stack.h"
#include "storage/page/unversioned_page.h"

namespace TextSearch {

class Table {
public:
    using TombstoneStackType = DiskIntStack<uint64_t>;

    static constexpr char TOMBSTONES_FILENAME[] = "table.tombstones.dat";

    struct Header {
        // the number of columns in the table.
        uint64_t column_count;
        // page pointer (page number + page offset) to the first unused byte
        uint64_t end_page_ptr;
    };

    static std::unique_ptr<Table> create(std::filesystem::path path, uint64_t column_count);

    static std::unique_ptr<Table> load(std::filesystem::path path);

    ~Table();

    // Inserts a row into the table and return the table pointer to the inserted row
    uint64_t insert(std::vector<uint64_t> values);

    // Obtains the row pointed to by page_pointer
    std::vector<uint64_t> get(uint64_t page_pointer);

    // Removes the row pointed to by table_pointer
    void remove(uint64_t table_pointer);

private:
    explicit Table(FileId file_id, FileId tombstones_file_id, UPage& first_page);

    // FileId of the file containing the table
    const FileId file_id;

    // The first page of the table, used to save metadata
    UPage& first_page;

    Header* header;

    // The currently last page, to avoid having to obtain the page
    // for every consecutive insert
    UPage* current_insert_page;

    TombstoneStackType tombstones_stack;
};

} // namespace TextSearch
