#include "table.h"

#include <cmath>

#include "storage/index/text_search/utils.h"
#include "system/buffer_manager.h"
#include "system/file_manager.h"

namespace TextSearch {

Table::Table(std::filesystem::path path, uint64_t column_count) :
    file_id    (file_manager.get_file_id(path)),
    first_page (buffer_manager.get_unversioned_page(file_id, 0)),
    column_count (column_count)
{
    uint64_t max_column_count = std::floor(static_cast<double>(UPage::SIZE - HEADER_SIZE)
                                / sizeof(uint64_t));

    if (column_count > max_column_count) {
        throw std::runtime_error("Trying to create table with "
                               + std::to_string(column_count)
                               + " columns, maximum allowed is "
                               + std::to_string(max_column_count));
    }

    if (column_count == 0) {
        throw std::runtime_error("Trying to create table with 0 columns");
    }

    auto column_count_ptr = reinterpret_cast<unsigned char*>(first_page.get_bytes());
    end_page_pointer = column_count_ptr + COLUMN_COUNT_SIZE;

    auto disc_column_count = read_bytes(column_count_ptr, COLUMN_COUNT_SIZE);

    if (disc_column_count != 0) {
        // Already existing table
        if (disc_column_count != column_count) {
            throw std::runtime_error("Preexisting table has incorrect column count: "
                                   + std::to_string(column_count)
                                   + ", expected " + std::to_string(column_count));
        }
    } else {
        // New table
        *end_page_pointer = HEADER_SIZE;
        write_bytes(column_count_ptr, COLUMN_COUNT_SIZE, column_count);

        first_page.make_dirty();
    }
}


Table::~Table() {
    buffer_manager.unpin(first_page);

    if (current_page != nullptr) {
        buffer_manager.unpin(*current_page);
    }
}


uint64_t Table::insert(std::vector<uint64_t> values) {
    if (values.size() != column_count) {
        throw std::logic_error("Trying to insert tuple of size " + std::to_string(values.size())
                               + " into table with column count " + std::to_string(column_count));
    }

    auto page_number = *end_page_pointer / UPage::SIZE;
    auto page_offset = *end_page_pointer % UPage::SIZE;

    auto row_size = values.size() * sizeof(uint64_t);

    if (row_size + page_offset > UPage::SIZE) {
        // Not enough space left at the end of page
        page_number++;
        page_offset = 0;
        *end_page_pointer = page_number * UPage::SIZE;
    }

    if (current_page == nullptr) {
        // Page has not ben loaded
        current_page = &buffer_manager.get_unversioned_page(file_id, page_number);
    } else if (current_page->get_page_number() != page_number) {
        // Another page is currently loaded
        buffer_manager.unpin(*current_page);
        current_page = &buffer_manager.get_unversioned_page(file_id, page_number);
    }

    std::memcpy(current_page->get_bytes() + page_offset, values.data(), row_size);

    *end_page_pointer += row_size;
    current_page->make_dirty();
    first_page.make_dirty();

    return *end_page_pointer - row_size;
}


std::vector<uint64_t> Table::get(uint64_t page_pointer) {
    auto page_number = page_pointer / UPage::SIZE;
    auto page_offset = page_pointer % UPage::SIZE;

    auto row_size = column_count * sizeof(uint64_t);

    if (current_page == nullptr) {
        // Page has not ben loaded
        current_page = &buffer_manager.get_unversioned_page(file_id, page_number);
    } else if (current_page->get_page_number() != page_number) {
        // Another page is currently loaded
        buffer_manager.unpin(*current_page);
        current_page = &buffer_manager.get_unversioned_page(file_id, page_number);
    }

    std::vector<uint64_t> result(column_count);
    std::memcpy(result.data(), current_page->get_bytes() + page_offset, row_size);

    return result;
}


} // namespace TextSearch
