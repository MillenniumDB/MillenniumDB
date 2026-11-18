#include "bplus_tree_leaf.h"

#include <cstring>

#include "query/query_context.h"
#include "system/buffer_manager.h"

using namespace std;

template<std::size_t N>
BPlusTreeLeaf<N>::~BPlusTreeLeaf()
{
    if (page != nullptr)
        buffer_manager.unpin(*page);
}

template<std::size_t N>
BPlusTreeLeaf<N> BPlusTreeLeaf<N>::clone() const
{
    buffer_manager.pin(*page);
    return BPlusTreeLeaf<N>(page);
}

template<std::size_t N>
void BPlusTreeLeaf<N>::update_to_next_leaf()
{
    auto next_page_number = *next_leaf;

    assert(page->page_id.page_number != next_page_number);

    buffer_manager.unpin(*page);

    page = &buffer_manager.get_page_readonly(leaf_file_id, next_page_number);
    value_count = reinterpret_cast<uint32_t*>(page->get_bytes());
    next_leaf = reinterpret_cast<uint32_t*>(page->get_bytes() + sizeof(uint32_t));
    bitset_ptr = (unsigned char*) page->get_bytes() + 2 * sizeof(uint32_t);
    redundant_bytes = (unsigned char*) page->get_bytes() + 2 * sizeof(uint32_t) + N;

    unsigned char* bitset_ptr = (unsigned char*) page->get_bytes() + 2 * sizeof(uint32_t);

    int pos_bitset = 0;
    for (size_t i = 0; i < N; ++i) {
        for (int bit = 0; bit < 8; bit++) {
            redundant_bitset.set(pos_bitset++, (bitset_ptr[i] >> bit) & 1);
        }
    }
    redundant_count = redundant_bitset.count();
    records = (unsigned char*) page->get_bytes() + 2 * sizeof(uint32_t) + N + redundant_count;
}

template<std::size_t N>
void BPlusTreeLeaf<N>::upgrade_to_editable()
{
    if (buffer_manager.need_edit_version(*page)) {
        auto new_page = &buffer_manager.get_page_editable(leaf_file_id, page->get_page_number());
        buffer_manager.unpin(*page);
        page = new_page;

        value_count = reinterpret_cast<uint32_t*>(page->get_bytes());
        next_leaf = reinterpret_cast<uint32_t*>(page->get_bytes() + sizeof(uint32_t));
        bitset_ptr = (unsigned char*) page->get_bytes() + 2 * sizeof(uint32_t);
        redundant_bytes = (unsigned char*) page->get_bytes() + 2 * sizeof(uint32_t) + N;

        unsigned char* bitset_ptr = (unsigned char*) page->get_bytes() + 2 * sizeof(uint32_t);

        int pos_bitset = 0;
        for (size_t i = 0; i < N; ++i) {
            for (int bit = 0; bit < 8; bit++) {
                redundant_bitset.set(pos_bitset++, (bitset_ptr[i] >> bit) & 1);
            }
        }
        redundant_count = redundant_bitset.count();
        records = (unsigned char*) page->get_bytes() + 2 * sizeof(uint32_t) + N + redundant_count;
    }
}

template<std::size_t N>
void BPlusTreeLeaf<N>::set_record(uint_fast32_t pos, Record<N>& out) const
{
    unsigned char* out_char = (unsigned char*) &out;

    unsigned char* current_record = records + pos * (N * sizeof(uint64_t) - redundant_count);
    size_t redundant_pos = 0;
    size_t unique_pos = 0;

    for (size_t i = 0; i < N * sizeof(uint64_t); ++i) {
        if (redundant_bitset[i]) {
            out_char[i] = redundant_bytes[redundant_pos];
            redundant_pos++;
        } else {
            out_char[i] = current_record[unique_pos];
            unique_pos++;
        }
    }
}

template<std::size_t N>
Record<N> BPlusTreeLeaf<N>::get_record(uint_fast32_t pos) const
{
    Record<N> out;
    set_record(pos, out);
    return out;
}

template<std::size_t N>
void BPlusTreeLeaf<N>::set_redundant_record(Record<N>& out) const
{
    unsigned char* out_char = (unsigned char*) &out;
    size_t redundant_pos = 0;

    for (size_t i = 0; i < N * sizeof(uint64_t); ++i) {
        if (redundant_bitset[i]) {
            out_char[i] = redundant_bytes[redundant_pos];
            redundant_pos++;
        }
    }
}

template<std::size_t N>
void BPlusTreeLeaf<N>::update_record(uint_fast32_t pos, Record<N>& out) const
{
    unsigned char* out_char = (unsigned char*) &out;

    unsigned char* current_record = records + pos * (N * sizeof(uint64_t) - redundant_count);
    size_t unique_pos = 0;

    for (size_t i = 0; i < N * sizeof(uint64_t); ++i) {
        if (!redundant_bitset[i]) {
            out_char[i] = current_record[unique_pos];
            unique_pos++;
        }
    }
}

template<std::size_t N>
bool BPlusTreeLeaf<N>::delete_record(const Record<N>& record)
{
    if (*value_count == 0) {
        return false;
    }

    uint_fast32_t index = search_index(record);
    if (equal_record(record, index)) {
        upgrade_to_editable();
        --(*value_count);
        for (auto i = index; i < (*value_count); i++) {
            for (uint_fast32_t j = 0; j < N * 8 - redundant_count; j++) {
                records[i * (N * 8 - redundant_count) + j] = records[(i + 1) * (N * 8 - redundant_count) + j];
            }
        }
        return true;
    } else {
        return false;
    }
}

template<std::size_t N>
uint32_t BPlusTreeLeaf<N>::get_needed_page_size(std::bitset<N * 8> bitset, uint32_t n_records)
{
    return 2 * sizeof(uint32_t) + N + bitset.count() + n_records * (sizeof(uint64_t) * N - bitset.count());
}

template<std::size_t N>
std::bitset<N * 8> BPlusTreeLeaf<N>::create_new_bitset(const Record<N>& reference, uint64_t from, uint64_t to)
{
    std::bitset<N * 8> new_bitset;
    new_bitset.set();

    unsigned char* record_char_ptr = (unsigned char*) &reference;
    Record<N> current_record;
    set_redundant_record(current_record);

    for (uint32_t i = from; i < to; ++i) {
        update_record(i, current_record);
        char* record_char = (char*) &current_record;

        for (size_t j = 0; j < N * 8; ++j) {
            if (new_bitset[j] && record_char[j] != record_char_ptr[j]) {
                new_bitset.set(j, 0);
            }
        }
    }
    return new_bitset;
}

template<std::size_t N>
void BPlusTreeLeaf<N>::compress_to_buffer(
    uint8_t* compression_buffer,
    const std::bitset<N * 8>& bitset,
    size_t from,
    size_t to
) const
{
    uint8_t* current_write_byte = compression_buffer;
    Record<N> current_record;
    set_redundant_record(current_record);

    for (size_t i = from; i < to; ++i) {
        update_record(i, current_record);
        auto record_bytes = (uint8_t*) &(current_record);

        for (size_t j = 0; j < N * 8; ++j) {
            if (!bitset[j]) {
                *current_write_byte = record_bytes[j];
                current_write_byte++;
            }
        }
    }
}

template<std::size_t N>
void BPlusTreeLeaf<N>::recompress_records(
    std::bitset<N * 8>& bitset,
    size_t new_value_count,
    const uint8_t* uncompressed_records
)
{
    // update the bitset
    redundant_bitset = bitset;
    redundant_count = bitset.count();
    unsigned long bits_ul = bitset.to_ulong();
    memcpy(bitset_ptr, &bits_ul, N);

    // write redundant bytes
    uint8_t* current_write_byte = redundant_bytes;
    for (uint64_t i = 0; i < N * 8; ++i) {
        if (redundant_bitset[i]) {
            *current_write_byte = uncompressed_records[i];
            current_write_byte++;
        }
    }

    // update the records
    records = redundant_bytes + redundant_count;
    for (uint64_t i = 0; i < new_value_count; ++i) {
        current_write_byte = &records[i * (N * 8 - redundant_count)];
        for (size_t j = 0; j < N * 8; ++j) {
            if (!redundant_bitset[j]) {
                *current_write_byte = uncompressed_records[i * (N * 8) + j];
                current_write_byte++;
            }
        }
    }

    *value_count = new_value_count;
}

template<std::size_t N>
unique_ptr<BPlusTreeSplit<N>> BPlusTreeLeaf<N>::insert(const Record<N>& record, bool& error)
{
    const unsigned char* const new_record_bytes = (unsigned char*) &record;

    if (*value_count == 0) {
        upgrade_to_editable();

        // If leaf was empty, now every byte is redundant
        redundant_bitset.set();
        redundant_count = N * 8;
        unsigned long bits_ul = redundant_bitset.to_ulong(); // TODO: probar aun cuando no hay compresion
        memcpy(bitset_ptr, &bits_ul, N); // TODO: probar aun cuando no hay compresion

        records = redundant_bytes + redundant_count;

        for (uint_fast32_t i = 0; i < N * 8; i++) {
            redundant_bytes[i] = new_record_bytes[i];
        }
        *value_count = 1;
        error = false;
        return nullptr;
    }
    const auto index = search_index(record);

    // avoid inserting duplicated record
    if (index != *value_count && equal_record(record, index)) {
        error = true;
        return nullptr;
    }

    error = false;
    upgrade_to_editable();

    const auto first_record = get_record(0);
    unsigned char* first_record_bytes = (unsigned char*) &first_record;

    std::bitset<N * 8> new_bitset = redundant_bitset;
    for (size_t i = 0; i < N * 8; ++i) {
        if (new_record_bytes[i] != first_record_bytes[i]) {
            new_bitset.set(i, 0);
        }
    }

    auto needed_size = get_needed_page_size(new_bitset, *value_count + 1);
    if (needed_size < Page::SIZE) {
        if (new_bitset == redundant_bitset) {
            // If the bitsets are equal, then the new record is added directly
            shift_right_records(index, *value_count - 1);

            uint8_t* current_write_byte = &records[index * (N * 8 - redundant_count)];
            for (uint_fast32_t i = 0; i < N * 8; i++) {
                if (!redundant_bitset[i]) {
                    *current_write_byte = new_record_bytes[i];
                    current_write_byte++;
                }
            }
        } else {
            // update the bitset
            redundant_bitset = new_bitset;
            redundant_count = new_bitset.count();
            unsigned long bits_ul = new_bitset.to_ulong();
            memcpy(bitset_ptr, &bits_ul, N);

            // write redundant bytes
            uint8_t* current_write_byte = redundant_bytes;
            for (uint64_t i = 0; i < N * 8; ++i) {
                if (redundant_bitset[i]) {
                    *current_write_byte = first_record_bytes[i];
                    current_write_byte++;
                }
            }

            // update the records with new compression
            QueryContext& query_ctx = get_query_ctx();
            auto compression_buffer = reinterpret_cast<uint8_t*>(query_ctx.get_buffer1());
            compress_to_buffer(compression_buffer, redundant_bitset, 0, *value_count);
            records = redundant_bytes + redundant_count;
            memcpy(records, compression_buffer, *value_count * (N * 8 - redundant_count));

            shift_right_records(index, *value_count - 1);

            // write the new record
            current_write_byte = &records[index * (N * 8 - redundant_count)];
            for (uint_fast32_t i = 0; i < N * 8; i++) {
                if (!redundant_bitset[i]) {
                    *current_write_byte = new_record_bytes[i];
                    current_write_byte++;
                }
            }
        }
        ++*value_count;
        return nullptr;
    }

    // Split is needed

    // 8+N is the size of the value_count + next_leaf + bitset.
    // 2036 is the maximum number of records in the leaf.
    static_assert(
        8 + N + 2036 * N * 8 <= QueryContext::buffer_size,
        "buffer in BPTreeLeaf is not big enough"
    );
    QueryContext& query_ctx = get_query_ctx();
    auto buffer = reinterpret_cast<uint8_t*>(query_ctx.get_buffer1());

    // decompress in buffer the range [0, index)
    for (size_t i = 0; i < index; ++i) {
        size_t redundant_pos = 0;
        size_t unique_pos = 0;
        for (size_t j = 0; j < N * 8; ++j) {
            if (redundant_bitset[j]) {
                buffer[i * (N * 8) + j] = redundant_bytes[redundant_pos];
                redundant_pos++;
            } else {
                buffer[i * (N * 8) + j] = records[i * (N * 8 - redundant_count) + unique_pos];
                unique_pos++;
            }
        }
    }

    // add the new record to the buffer
    memcpy(&buffer[index * (N * 8)], &record, N * 8);

    // decompress in buffer the range [index + 1, *value_count]
    // include *value_count because it still does not increment at this point
    for (size_t i = index + 1; i <= *value_count; ++i) {
        size_t redundant_pos = 0;
        size_t unique_pos = 0;
        for (size_t j = 0; j < N * 8; ++j) {
            if (redundant_bitset[j]) {
                buffer[i * (N * 8) + j] = redundant_bytes[redundant_pos];
                redundant_pos++;
            } else {
                buffer[i * (N * 8) + j] = records[i * (N * 8 - redundant_count) + unique_pos];
                unique_pos++;
            }
        }
    }

    // calculate the number of records on each leaf
    uint8_t* first_uncompressed_records = buffer;
    uint8_t* last_uncompressed_records = buffer + *value_count * (N * 8);

    std::bitset<N * 8> bitset_tmp;
    bitset_tmp.set();

    uint64_t possible_records_lhs = 0;
    uint64_t possible_records_rhs = 0;

    // try to calculate how many contiguous records is possible to put in a new leaf
    // counting from less to high
    for (uint64_t i = 0; i < *value_count + 1; ++i) {
        uint8_t* current_record = buffer + i * (N * 8);

        for (uint64_t j = 0; j < N * 8; ++j) {
            if (bitset_tmp[j] && current_record[j] != first_uncompressed_records[j]) {
                bitset_tmp.set(j, 0);
            }
        }
        if (get_needed_page_size(bitset_tmp, i) < Page::SIZE) {
            possible_records_lhs++;
        } else {
            break;
        }
    }

    bitset_tmp.set();

    // try to calculate how many contiguous records is possible to put in a new leaf
    // counting from last to low
    for (uint64_t i = 0; i < *value_count + 1; ++i) {
        uint8_t* current_record = buffer + (*value_count - i) * (N * 8);

        for (uint64_t j = 0; j < N * 8; ++j) {
            if (bitset_tmp[j] && current_record[j] != last_uncompressed_records[j]) {
                bitset_tmp.set(j, 0);
            }
        }
        if (get_needed_page_size(bitset_tmp, i) < Page::SIZE) {
            possible_records_rhs++;
        } else {
            break;
        }
    }

    if (possible_records_lhs + possible_records_rhs >= *value_count + 1) {
        // Case 1: the leaf does not require a double split
        auto& right_page = buffer_manager.append_page(leaf_file_id);
        auto right_leaf = BPlusTreeLeaf<N>(&right_page);

        *right_leaf.next_leaf = *next_leaf;
        *next_leaf = right_leaf.page->get_page_number();

        auto n_records_lhs = (*value_count + 1 - possible_records_lhs + possible_records_rhs) / 2;
        auto n_records_rhs = *value_count + 1 - n_records_lhs;

        // recalculate the left and right bitsets
        std::bitset<N * 8> lhs_bitset;
        std::bitset<N * 8> rhs_bitset;
        lhs_bitset.set();
        rhs_bitset.set();

        for (uint64_t i = 0; i < n_records_lhs; ++i) {
            uint8_t* current_record = buffer + i * (N * 8);
            for (size_t j = 0; j < N * 8; ++j) {
                if (lhs_bitset[j] && current_record[j] != first_uncompressed_records[j]) {
                    lhs_bitset.set(j, 0);
                }
            }
        }

        for (uint64_t i = 0; i < n_records_rhs; ++i) {
            uint8_t* current_record = buffer + (*value_count - i) * (N * 8);
            for (size_t j = 0; j < N * 8; ++j) {
                if (lhs_bitset[j] && current_record[j] != last_uncompressed_records[j]) {
                    lhs_bitset.set(j, 0);
                }
            }
        }

        this->recompress_records(lhs_bitset, n_records_lhs, buffer);
        right_leaf.recompress_records(rhs_bitset, n_records_rhs, buffer + n_records_lhs * (N * 8));

        return std::make_unique<BPlusTreeSplit<N>>(right_leaf.get_record(0), right_page.get_page_number());
    } else {
        // Case 2: the leaf requires a double split
        // Transform [THIS]->[OLD_NEXT] into [THIS]->[MIDDLE]->[RIGHT]->[OLD_NEXT]
        auto& middle_page = buffer_manager.append_page(leaf_file_id);
        auto middle_leaf = BPlusTreeLeaf<N>(&middle_page);

        auto& right_page = buffer_manager.append_page(leaf_file_id);
        auto right_leaf = BPlusTreeLeaf<N>(&right_page);

        *right_leaf.next_leaf = *next_leaf;
        *middle_leaf.next_leaf = right_leaf.page->get_page_number();
        *next_leaf = middle_leaf.page->get_page_number();

        // middle left
        uint64_t n_records_lhs = possible_records_lhs;
        uint64_t n_records_rhs = possible_records_rhs;
        uint64_t n_records_middle = (*value_count + 1) - (n_records_lhs + n_records_rhs);

        // not the middle of the list, but the first record to MIDDLE leaf
        uint8_t* middle_uncompressed = buffer + n_records_lhs * (N * 8);

        std::bitset<N * 8> middle_bitset;
        middle_bitset.set();
        for (uint64_t i = 0; i < n_records_middle; ++i) {
            unsigned char* current_record = buffer + (n_records_lhs + i) * (N * 8);
            for (size_t j = 0; j < N * 8; ++j) {
                if (middle_bitset[j] && current_record[j] != middle_uncompressed[j]) {
                    middle_bitset.set(j, 0);
                }
            }
        }

        // recalculate the left and right bitsets
        std::bitset<N * 8> lhs_bitset;
        std::bitset<N * 8> rhs_bitset;
        lhs_bitset.set();
        rhs_bitset.set();

        for (uint64_t i = 0; i < n_records_lhs; ++i) {
            uint8_t* current_record = buffer + i * (N * 8);
            for (size_t j = 0; j < N * 8; ++j) {
                if (lhs_bitset[j] && current_record[j] != first_uncompressed_records[j]) {
                    lhs_bitset.set(j, 0);
                }
            }
        }

        for (uint64_t i = 0; i < n_records_rhs; ++i) {
            uint8_t* current_record = buffer + (*value_count - i) * (N * 8);
            for (size_t j = 0; j < N * 8; ++j) {
                if (lhs_bitset[j] && current_record[j] != last_uncompressed_records[j]) {
                    lhs_bitset.set(j, 0);
                }
            }
        }

        this->recompress_records(lhs_bitset, n_records_lhs, buffer);
        middle_leaf.recompress_records(middle_bitset, n_records_middle, middle_uncompressed);
        right_leaf.recompress_records(
            rhs_bitset,
            n_records_rhs,
            buffer + (n_records_lhs + n_records_middle) * (N * 8)
        );

        return std::make_unique<BPlusTreeSplit<N>>(
            middle_leaf.get_record(0),
            middle_page.get_page_number(),
            right_leaf.get_record(0),
            right_page.get_page_number()
        );
    }
}

// returns the position of the minimum key greater or equal than the record given.
// if there is no such key, returns (to + 1)
template<std::size_t N>
uint_fast32_t BPlusTreeLeaf<N>::search_index(const Record<N>& record) const noexcept
{
    int_fast32_t from = 0;
    int_fast32_t to = static_cast<int_fast32_t>(*value_count) - 1;
    Record<N> search_record;
    set_redundant_record(search_record);
search_index_begin:
    if (from < to) {
        auto middle = (from + to) / 2;
        update_record(middle, search_record);

        for (uint_fast32_t i = 0; i < N; i++) {
            if (record[i] < search_record[i]) { // record is smaller
                to = middle - 1;
                goto search_index_begin;
            } else if (record[i] > search_record[i]) { // record is greater
                from = middle + 1;
                goto search_index_begin;
            }
        }
        // record is equal
        return middle;
    }

    // from >= to
    update_record(from, search_record);

    for (uint_fast32_t i = 0; i < N; ++i) {
        if (record[i] < search_record[i]) {
            return from;
        } else if (record[i] > search_record[i]) {
            return from + 1;
        }
        // continue if they were equal
    }
    return from;
}

template<std::size_t N>
void BPlusTreeLeaf<N>::shift_right_records(int_fast32_t from, int_fast32_t to)
{
    uint64_t record_step = N * 8 - redundant_count;

    for (auto i = to; i >= from; i--) {
        for (size_t j = 0; j < record_step; j++) {
            records[(i + 1) * record_step + j] = records[i * record_step + j];
        }
    }
}

template<std::size_t N>
bool BPlusTreeLeaf<N>::equal_record(const Record<N>& record, uint_fast32_t index)
{
    unsigned char* record_char_ptr = (unsigned char*) &(record);
    int unique_size = N * 8 - redundant_count;

    size_t redundant_pos = 0;
    size_t unique_pos = 0;

    for (size_t i = 0; i < N * 8; ++i) {
        if (redundant_bitset[i]) {
            if (redundant_bytes[redundant_pos] != record_char_ptr[i]) {
                return false;
            }
            redundant_pos++;
        } else {
            if (records[index * unique_size + unique_pos] != record_char_ptr[i]) {
                return false;
            }
            unique_pos++;
        }
    }
    return true;
}

template<std::size_t N>
bool BPlusTreeLeaf<N>::check_range(const Record<N>& r) const
{
    if (*value_count == 0) {
        return false;
    }
    auto min = get_record(0);
    auto max = get_record(*value_count - 1);

    return min <= r && r <= max;
}

template<std::size_t N>
void BPlusTreeLeaf<N>::print(std::ostream& os) const
{
    os << "Printing Leaf:\n";
    for (uint_fast32_t i = 0; i < (*value_count); i++) {
        os << "  (";
        unsigned char* current_record = records + i * (N * 8 - redundant_count);
        uint64_t pos_redundant = 0;
        uint64_t pos_unique = 0;
        for (uint_fast32_t j = 0; j < N * 8; ++j) {
            if (j == 8 || j == 16) {
                os << ", ";
            }
            if (redundant_bitset[j]) {
                os << redundant_bytes[pos_redundant];
                pos_redundant++;
            } else {
                os << current_record[pos_unique];
                pos_unique++;
            }
        }
        os << ")\n";
    }
}

template<std::size_t N>
bool BPlusTreeLeaf<N>::check(std::ostream& os) const
{
    if ((*value_count) == 0) {
        if (page->get_page_number() == 0) {
            os << "  WARNING: empty leaf. Ok only if the b+tree is empty.\n";
        } else {
            os << "  ERROR: BPlusTreeLeaf empty (page: " << page->get_page_number() << ")\n";
            return false;
        }
    } else {
        // check keys are ordered
        Record<N> x = get_record(0);

        for (uint32_t i = 0; i < N; ++i) {
            if (x[i] == 0xFFFF'FFFF'FFFF'FFFF) {
                os << "  ERROR: record not_found(0xFFFF'FFFF'FFFF'FFFF) at BPlusTreeLeaf\n";
                return false;
            }
        }

        Record<N> y;
        for (uint_fast32_t k = 1; k < (*value_count); k++) {
            set_record(k, y);
            if (y <= x) {
                os << "  ERROR: bad record order at BPlusTreeLeaf(page: " << page->get_page_number() << ")\n";
                for (size_t n = 0; n < N; n++) {
                    os << "\t" << x[n];
                }
                os << "\n";

                for (size_t n = 0; n < N; n++) {
                    os << "\t" << y[n];
                }
                os << "\n";

                // print(os);
                return false;
            }
            x = y;
        }
    }
    return true;
}

template class BPlusTreeLeaf<1>;
template class BPlusTreeLeaf<2>;
template class BPlusTreeLeaf<3>;
template class BPlusTreeLeaf<4>;
