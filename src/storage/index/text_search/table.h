#pragma once

#include <filesystem>
#include <vector>

#include "storage/disk_int_stack.h"

namespace TextSearch {

// first 8 bytes (uint64) : column_count
// next  8 bytes (uint64) : end table_pointer (first empty, not considering removed)
class Table {
public:
    static constexpr char TOMBSTONES_FILENAME[] = "table.tombstones.dat";

    Table(const std::filesystem::path& path, uint64_t column_count);

    // Inserts a row into the table and return the table pointer to the inserted row
    uint64_t insert(const std::vector<uint64_t>& values);

    // Obtains the row pointed to by table_pointer
    std::vector<uint64_t> get(uint64_t table_pointer);

    // Removes the row pointed to by table_pointer
    void remove(uint64_t table_pointer);

private:
    // FileId of the file containing the table
    const FileId file_id;

    DiskIntStack<uint64_t> tombstones_stack;
};

} // namespace TextSearch
