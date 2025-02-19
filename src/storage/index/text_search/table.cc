#include "table.h"

#include <cmath>

#include "system/buffer_manager.h"
#include "system/file_manager.h"

namespace TextSearch {

std::unique_ptr<Table> Table::create(std::filesystem::path path, uint64_t column_count)
{
    if (column_count < 1) {
        throw std::runtime_error("Trying to create table with 0 columns");
    }

    const auto max_column_count = std::floor(
        static_cast<double>(UPage::SIZE - sizeof(Table::Header)) / sizeof(uint64_t)
    );
    if (column_count > max_column_count) {
        throw std::runtime_error(
            "Trying to create table with " + std::to_string(column_count) + " columns, maximum allowed is "
            + std::to_string(max_column_count)
        );
    }

    const auto table_file_id = file_manager.get_file_id(path);
    auto& first_page = buffer_manager.append_unversioned_page(table_file_id);

    Table::Header header {};
    header.column_count = column_count;
    header.end_page_ptr = sizeof(Table::Header);
    memcpy(first_page.get_bytes(), &header, sizeof(Table::Header));

    const auto tombstones_file_id = file_manager.get_file_id(path.parent_path() / Table::TOMBSTONES_FILENAME);
    Table::TombstoneStackType::create(tombstones_file_id);

    return std::unique_ptr<Table>(new Table(table_file_id, tombstones_file_id, first_page));
}

std::unique_ptr<Table> Table::load(std::filesystem::path path)
{
    const auto table_file_id = file_manager.get_file_id(path);
    const auto tombstones_file_id = file_manager.get_file_id(path.parent_path() / Table::TOMBSTONES_FILENAME);
    auto& first_page = buffer_manager.get_unversioned_page(table_file_id, 0);

    return std::unique_ptr<Table>(new Table(table_file_id, tombstones_file_id, first_page));
}

Table::Table(FileId file_id_, FileId tombstones_file_id, UPage& first_page_) :
    file_id { file_id_ },
    first_page { first_page_ },
    header { reinterpret_cast<Header*>(first_page.get_bytes()) },
    current_insert_page { &buffer_manager.get_unversioned_page(file_id, header->end_page_ptr / UPage::SIZE) },
    tombstones_stack { tombstones_file_id }
{
    assert(header->column_count > 0);
    assert(header->column_count < std::floor(static_cast<double>(UPage::SIZE - sizeof(Table::Header))));
}

Table::~Table()
{
    buffer_manager.unpin(first_page);
    buffer_manager.unpin(*current_insert_page);
}

uint64_t Table::insert(std::vector<uint64_t> values)
{
    if (values.size() != header->column_count) {
        throw std::logic_error(
            "Trying to insert tuple of size " + std::to_string(values.size())
            + " into table with column count " + std::to_string(header->column_count)
        );
    }

    if (!tombstones_stack.empty()) {
        // A tombstone is available and the values to insert must fit int the available space
        const auto page_pointer = tombstones_stack.pop();
        const auto page_number = page_pointer / UPage::SIZE;
        const auto page_offset = page_pointer % UPage::SIZE;

        if (current_insert_page->get_page_number() != page_number) {
            // Another page is currently loaded
            buffer_manager.unpin(*current_insert_page);
            current_insert_page = &buffer_manager.get_or_append_unversioned_page(file_id, page_number);
        }

        std::memcpy(
            current_insert_page->get_bytes() + page_offset,
            values.data(),
            values.size() * sizeof(uint64_t)
        );

        current_insert_page->make_dirty();
        return page_pointer;
    }

    auto page_number = header->end_page_ptr / UPage::SIZE;
    auto page_offset = header->end_page_ptr % UPage::SIZE;

    const auto row_size = values.size() * sizeof(uint64_t);

    if (row_size + page_offset > UPage::SIZE) {
        // Not enough space left at the end of page
        ++page_number;
        page_offset = 0;
        header->end_page_ptr = page_number * UPage::SIZE;
    }

    if (current_insert_page->get_page_number() != page_number) {
        // Another page is currently loaded
        buffer_manager.unpin(*current_insert_page);
        current_insert_page = &buffer_manager.get_or_append_unversioned_page(file_id, page_number);
    }

    std::memcpy(current_insert_page->get_bytes() + page_offset, values.data(), row_size);

    header->end_page_ptr += row_size;
    current_insert_page->make_dirty();
    first_page.make_dirty();

    return header->end_page_ptr - row_size;
}

std::vector<uint64_t> Table::get(uint64_t page_pointer)
{
    const auto page_number = page_pointer / UPage::SIZE;
    const auto page_offset = page_pointer % UPage::SIZE;
    const auto row_size = header->column_count * sizeof(uint64_t);

    auto& page = buffer_manager.get_unversioned_page(file_id, page_number);
    std::vector<uint64_t> result(header->column_count);
    std::memcpy(result.data(), page.get_bytes() + page_offset, row_size);
    buffer_manager.unpin(page);

    return result;
}

void Table::remove(uint64_t table_pointer)
{
    // It is assumed that this table pointer wont be accessed unexpectedly afterwards,
    // so we can just mark it as a tombstone
    tombstones_stack.push(table_pointer);
}

} // namespace TextSearch
