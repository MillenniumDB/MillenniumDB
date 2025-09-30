#include "table.h"

#include "system/buffer_manager.h"
#include "system/file_manager.h"

using namespace TextSearch;

constexpr uint64_t int_floor(float f)
{
    const uint64_t i = static_cast<uint64_t>(f);
    return f < i ? i - 1 : i;
}

Table::Table(const std::filesystem::path& path, uint64_t column_count) :
    file_id(file_manager.get_file_id(path)),
    tombstones_stack(file_manager.get_file_id(path.parent_path() / Table::TOMBSTONES_FILENAME))
{
    if (column_count < 1) {
        throw std::runtime_error("Trying to create table with 0 columns");
    }
    constexpr auto max_column_count = int_floor(
        static_cast<double>(Page::SIZE - 2 * sizeof(uint64_t)) / sizeof(uint64_t)
    );
    if (column_count > max_column_count) {
        throw std::runtime_error(
            "Trying to create table with " + std::to_string(column_count) + " columns, maximum allowed is "
            + std::to_string(max_column_count)
        );
    }

    auto& first_page = buffer_manager.get_page_readonly(file_id, 0);
    const auto page_col_count = *reinterpret_cast<uint64_t*>(first_page.get_bytes());
    if (page_col_count == 0) {
        // column count cannot be 0, so this page is new
        auto& first_page_editable = buffer_manager.get_page_editable(file_id, 0);

        auto column_count_ptr = reinterpret_cast<uint64_t*>(first_page_editable.get_bytes());
        auto end_table_ptr = column_count_ptr + 1;

        *column_count_ptr = column_count;
        *end_table_ptr = 2 * sizeof(uint64_t);

        buffer_manager.unpin(first_page_editable);
    } else if (page_col_count != column_count) {
        throw std::runtime_error(
            "Trying to create table with " + std::to_string(column_count) + " columns, existing table has "
            + std::to_string(max_column_count)
        );
    }
    buffer_manager.unpin(first_page);
}

std::vector<uint64_t> Table::get(uint64_t table_pointer)
{
    auto& first_page = buffer_manager.get_page_readonly(file_id, 0);
    const auto column_count = *reinterpret_cast<uint64_t*>(first_page.get_bytes());
    buffer_manager.unpin(first_page);

    const auto page_number = table_pointer / Page::SIZE;
    const auto page_offset = table_pointer % Page::SIZE;
    const auto row_size = column_count * sizeof(uint64_t);

    std::vector<uint64_t> res(column_count);

    auto& page = buffer_manager.get_page_readonly(file_id, page_number);
    std::memcpy(res.data(), page.get_bytes() + page_offset, row_size);
    buffer_manager.unpin(page);

    return res;
}

uint64_t Table::insert(const std::vector<uint64_t>& values)
{
    auto& first_page = buffer_manager.get_page_readonly(file_id, 0);
    const auto column_count = *reinterpret_cast<uint64_t*>(first_page.get_bytes());
    buffer_manager.unpin(first_page);

    if (values.size() != column_count) {
        throw std::logic_error(
            "Trying to insert tuple of size " + std::to_string(values.size())
            + " into table with column count " + std::to_string(column_count)
        );
    }

    std::optional<uint64_t> available_tombstone = tombstones_stack.try_pop();
    if (available_tombstone.has_value()) {
        // A tombstone is available and the values to insert must fit int the available space
        const auto page_pointer = available_tombstone.value();
        const auto page_number = page_pointer / Page::SIZE;
        const auto page_offset = page_pointer % Page::SIZE;

        auto& insert_page = buffer_manager.get_page_editable(file_id, page_number);

        std::memcpy(insert_page.get_bytes() + page_offset, values.data(), values.size() * sizeof(uint64_t));

        buffer_manager.unpin(insert_page);
        return page_pointer;
    }

    // this time first page is editable
    auto& first_page_editable = buffer_manager.get_page_editable(file_id, 0);

    auto column_count_ptr = reinterpret_cast<uint64_t*>(first_page_editable.get_bytes());

    auto page_number = *column_count_ptr / Page::SIZE;
    auto page_offset = *column_count_ptr % Page::SIZE;

    const auto row_size = values.size() * sizeof(uint64_t);

    if (row_size + page_offset > Page::SIZE) {
        // Not enough space left at the end of page
        page_number++;
        page_offset = 0;
        *column_count_ptr = page_number * Page::SIZE;
    }

    auto& insert_page = buffer_manager.get_page_editable(file_id, page_number);
    std::memcpy(insert_page.get_bytes() + page_offset, values.data(), row_size);

    auto res = *column_count_ptr;
    *column_count_ptr += row_size;

    buffer_manager.unpin(first_page_editable);
    buffer_manager.unpin(insert_page);

    return res;
}

void Table::remove(uint64_t table_pointer)
{
    // It is assumed that this table pointer wont be accessed unexpectedly afterwards,
    // so we can just mark it as a tombstone
    tombstones_stack.push(table_pointer);
}

