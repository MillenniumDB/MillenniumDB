#include "bplus_tree_leaf.h"

#include <cstring>

#include "storage/index/bplus_tree/bplus_tree.h"
#include "system/buffer_manager.h"
#include "query/query_context.h"

using namespace std;

template <std::size_t N>
BPlusTreeLeaf<N>::~BPlusTreeLeaf() {
    if (page != nullptr)
        buffer_manager.unpin(*page);
}


template <std::size_t N>
BPlusTreeLeaf<N> BPlusTreeLeaf<N>::clone() const {
    buffer_manager.pin(*page);
    return BPlusTreeLeaf<N>(page);
}


template <std::size_t N>
void BPlusTreeLeaf<N>::update_to_next_leaf() {
    auto next_page_number = *next_leaf;

    assert(page->page_id.page_number != next_page_number);

    buffer_manager.unpin(*page);

    page        = &buffer_manager.get_page_readonly(leaf_file_id, next_page_number);
    value_count = reinterpret_cast<uint32_t*>(page->get_bytes());
    next_leaf   = reinterpret_cast<uint32_t*>(page->get_bytes() + sizeof(uint32_t));
    bitset_ptr      = (unsigned char*) page->get_bytes() + 2 * sizeof(uint32_t);
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


template <std::size_t N>
void BPlusTreeLeaf<N>::upgrade_to_editable() {
    if (buffer_manager.need_edit_version(*page)) {

        auto new_page = &buffer_manager.get_page_editable(leaf_file_id, page->get_page_number());
        buffer_manager.unpin(*page);
        page = new_page;

        value_count = reinterpret_cast<uint32_t*>(page->get_bytes());
        next_leaf   = reinterpret_cast<uint32_t*>(page->get_bytes() + sizeof(uint32_t));
        bitset_ptr      = (unsigned char*) page->get_bytes() + 2 * sizeof(uint32_t);
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


template <std::size_t N>
void BPlusTreeLeaf<N>::set_record(uint_fast32_t pos, Record<N>& out) const {
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


template <std::size_t N>
Record<N> BPlusTreeLeaf<N>::get_record(uint_fast32_t pos) const {
    Record<N> out;
    set_record(pos, out);
    return out;
}


template <std::size_t N>
void BPlusTreeLeaf<N>::set_redundant_record(Record<N>& out) const {
    unsigned char* out_char = (unsigned char*) &out;
    size_t redundant_pos = 0;

    for (size_t i = 0; i < N * sizeof(uint64_t); ++i) {
        if (redundant_bitset[i]) {
            out_char[i] = redundant_bytes[redundant_pos];
            redundant_pos++;
        }
    }
}

template <std::size_t N>
void BPlusTreeLeaf<N>::update_record(uint_fast32_t pos, Record<N>& out) const {
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


template <std::size_t N>
bool BPlusTreeLeaf<N>::delete_record(const Record<N>& record) {
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


template <std::size_t N>
uint32_t BPlusTreeLeaf<N>::get_page_size(std::bitset<N * 8> bitset, uint32_t n_records) {
    return 2 * sizeof(uint32_t) + N + bitset.count() + n_records * (sizeof(uint64_t) * N - bitset.count());
}


template <std::size_t N>
std::bitset<N * 8> BPlusTreeLeaf<N>::create_new_bitset(const Record<N>& reference, uint64_t from, uint64_t to) {
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
void BPlusTreeLeaf<N>::compress_to_buffer(unsigned char*     compression_buffer,
                                          std::bitset<N * 8> bitset,
                                          uint64_t           from,
                                          uint64_t           to) {
    uint64_t compression_pos = 0;
    Record<N> current_record;
    set_redundant_record(current_record);

    for (uint32_t i = from; i < to; ++i) {
        update_record(i, current_record);
        unsigned char* record_char = (unsigned char*) &(current_record);

        for (size_t j = 0; j < N * 8; ++j) {
            if (!bitset[j]) {
                compression_buffer[compression_pos] = record_char[j];
                compression_pos++;
            }
        }
    }
}

template<std::size_t N>
void BPlusTreeLeaf<N>::update_leaf(BPlusTreeLeaf<N>&   leaf,
                                   std::bitset<N * 8>& bitset,
                                   uint64_t            n_records,
                                   unsigned char*      buffer) {
    // update the bitset
    leaf.redundant_bitset = bitset;
    leaf.redundant_count = bitset.count();
    unsigned long bits_ul = bitset.to_ulong();
    memcpy(leaf.bitset_ptr, &bits_ul, N);

    // update the records
    leaf.records = leaf.redundant_bytes + leaf.redundant_count;
    for (uint64_t i = 0; i < n_records; ++i) {
        uint64_t unique_pos = 0;
        for (size_t j = 0; j < N * 8; ++j) {
            if (!leaf.redundant_bitset[j]) {
                leaf.records[i * (N * 8 - leaf.redundant_count) + unique_pos] = buffer[i * (N * 8) + j];
                unique_pos++;
            }
        }
    }

    // write redundant bytes
    uint64_t redundant_pos = 0;
    for (uint64_t i = 0; i < N * 8; ++i) {
        if (leaf.redundant_bitset[i]) {
            leaf.redundant_bytes[redundant_pos] = buffer[i];
            redundant_pos++;
        }
    }

    *leaf.value_count = n_records;
}


template <std::size_t N>
unique_ptr<BPlusTreeSplit<N>> BPlusTreeLeaf<N>::insert(const Record<N>& record, bool& error) {
    unsigned char* new_record_char_ptr = (unsigned char*) &record;

    if (*value_count == 0) {
        upgrade_to_editable();

        unsigned long bits_ul = 0xFFFFFFFF;
        memcpy(bitset_ptr, &bits_ul, N);
        redundant_count = N * 8;
        records = redundant_bytes + redundant_count;

        for (uint_fast32_t i = 0; i < N * 8; i++) {
            redundant_bytes[i] = new_record_char_ptr[i];
        }
        ++(*value_count);
        error = false;
        return nullptr;
    }
    uint_fast32_t index = search_index(record);

    // avoid inserting duplicated record
    if (index != *value_count && equal_record(record, index)) {
        error = true;
        return nullptr;
    }

    error = false;
    upgrade_to_editable();

    std::bitset<N * 8> new_bitset = redundant_bitset;

    const auto first_record = get_record(0);
    unsigned char* first_record_char = (unsigned char*) &first_record;

    for (size_t i = 0; i < N * 8; ++i) {
        if (new_record_char_ptr[i] != first_record_char[i]) {
            new_bitset.set(i, 0);
        }
    }

    if (get_page_size(new_bitset, (*value_count) + 1) < VPage::SIZE) {
        // If the bitsets are equal, then the new record is added
        if (new_bitset == redundant_bitset) {
            shift_right_records(index, *value_count - 1);

            uint64_t unique_pos = 0;
            for (uint_fast32_t i = 0; i < N * 8; i++) {
                if (!redundant_bitset[i]) {
                    records[index * (N * 8 - redundant_count) + unique_pos] = new_record_char_ptr[i];
                    unique_pos++;
                }
            }
            ++(*value_count);
        }
        // if the bitsets are different, then the records are compressed according to the new bitset
        else {
            QueryContext& query_ctx = get_query_ctx();
            unsigned char* compression_buffer = reinterpret_cast<unsigned char*>(query_ctx.get_buffer1());

            compress_to_buffer(compression_buffer, new_bitset, 0, *value_count);

            // update the bitset
            redundant_bitset = new_bitset;
            redundant_count = new_bitset.count();
            unsigned long bits_ul = new_bitset.to_ulong();
            memcpy(bitset_ptr, &bits_ul, N);

            // update the records
            records = redundant_bytes + redundant_count;
            memcpy(records, compression_buffer, *value_count * (N * 8 - redundant_count));

            // write redundant bytes
            uint64_t redundant_pos = 0;
            for (uint64_t i = 0; i < N * 8; ++i) {
                if (redundant_bitset[i]) {
                    redundant_bytes[redundant_pos] = first_record_char[i];
                    redundant_pos++;
                }
            }

            shift_right_records(index, *value_count - 1);
            ++(*value_count);

            // add the new record
            uint64_t unique_pos = 0;
            for (uint_fast32_t i = 0; i < N * 8; i++) {
                if (!redundant_bitset[i]) {
                    records[index * (N * 8 - redundant_count) + unique_pos] = new_record_char_ptr[i];
                    unique_pos++;
                }
            }
        }

        return nullptr;
    }

    // a split is needed
    // 8+N is the size of the value_count + next_leaf + bitset.
    // 2036 is the maximum number of records in the leaf.
    static_assert(8 + N + 2036 * N * 8 <= QueryContext::buffer_size, "buffer in BPTreeLeaf is not big enough");
    QueryContext& query_ctx = get_query_ctx();
    unsigned char* buffer = reinterpret_cast<unsigned char*>(query_ctx.get_buffer1());

    // decompress from 0 to index
    for (uint64_t i = 0; i < index; ++i) {
        uint64_t redundant_pos = 0;
        uint64_t unique_pos = 0;
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

    // decompress from index to value count
    for (uint64_t i = index; i < *value_count; ++i) {
        uint64_t redundant_pos = 0;
        uint64_t unique_pos = 0;
        for (size_t j = 0; j < N * 8; ++j) {
            if (redundant_bitset[j]) {
                buffer[(i + 1) * (N * 8) + j] = redundant_bytes[redundant_pos];
                redundant_pos++;
            } else {
                buffer[(i + 1) * (N * 8) + j] = records[i * (N * 8 - redundant_count) + unique_pos];
                unique_pos++;
            }
        }
    }

    // add the new record to the buffer
    memcpy(&buffer[index * (N * 8)], &record, N * 8);

    // calculate the number of records on each leaf
    unsigned char* left_reference = buffer;
    unsigned char* right_reference = buffer + *value_count * (N * 8);

    std::bitset<N * 8> left_bitset;
    std::bitset<N * 8> right_bitset;
    std::bitset<N * 8> bitset_tmp;
    left_bitset.set();
    right_bitset.set();
    bitset_tmp.set();

    uint64_t n_records_left = 0;
    uint64_t n_records_right = 0;

    for (uint64_t i = 0; i < *value_count + 1; ++i) {
        unsigned char* current_record = buffer + i * (N * 8);

        for (uint64_t j = 0; j < N * 8; ++j) {
            if (left_bitset[j] && current_record[j] != left_reference[j]) {
                bitset_tmp.set(j, 0);
            }
        }
        if (get_page_size(bitset_tmp, i + 1) > VPage::SIZE) {
            n_records_left = i;
            break;
        }
        left_bitset = bitset_tmp;
    }

    bitset_tmp.set();

    for (uint64_t i = 0; i < *value_count + 1; ++i) {
        unsigned char* current_record = buffer + (*value_count - i) * (N * 8);

        for (uint64_t j = 0; j < N * 8; ++j) {
            if (right_bitset[j] && current_record[j] != right_reference[j]) {
                bitset_tmp.set(j, 0);
            }
        }
        if (get_page_size(bitset_tmp, i + 1) > VPage::SIZE) {
            n_records_right = i;
            break;
        }
        right_bitset = bitset_tmp;
    }

    // Case 1: the leaf does not require a double split
    if (n_records_left + n_records_right >= *value_count + 1) {
        auto& right_page = buffer_manager.append_vpage(leaf_file_id);
        auto right_leaf = BPlusTreeLeaf<N>(&right_page);

        *right_leaf.next_leaf = *next_leaf;
        *next_leaf = right_leaf.page->get_page_number();

        // recalculate the left and right bitsets
        if (n_records_left + n_records_right > *value_count + 1) {
            n_records_left  = (*value_count + 1 - n_records_right + n_records_left) / 2;
            n_records_right = *value_count + 1 - n_records_left;

            left_bitset.set();
            for (uint64_t i = 0; i < n_records_left; ++i) {
                unsigned char* current_record = buffer + i * (N * 8);
                for (size_t j = 0; j < N * 8; ++j) {
                    if (left_bitset[j] && current_record[j] != left_reference[j]) {
                        left_bitset.set(j, 0);
                    }
                }
            }

            right_bitset.set();
            for (uint64_t i = 0; i < n_records_right; ++i) {
                unsigned char* current_record = buffer + (*value_count - i) * (N * 8);
                for (size_t j = 0; j < N * 8; ++j) {
                    if (right_bitset[j] && current_record[j] != right_reference[j]) {
                        right_bitset.set(j, 0);
                    }
                }
            }
        }

        // left leaf
        update_leaf(*this, left_bitset, n_records_left, buffer);

        // right leaf
        update_leaf(right_leaf, right_bitset, n_records_right, buffer + n_records_left * (N * 8));

        const auto first_right = right_leaf.get_record(0);
        return std::make_unique<BPlusTreeSplit<N>>(first_right, right_page.get_page_number());
    }

    // Case 2: the leaf requires a double split
    else {
        auto& middle_page = buffer_manager.append_vpage(leaf_file_id);
        auto middle_leaf = BPlusTreeLeaf<N>(&middle_page);

        auto& right_page = buffer_manager.append_vpage(leaf_file_id);
        auto right_leaf = BPlusTreeLeaf<N>(&right_page);

        *right_leaf.next_leaf = *next_leaf;
        *middle_leaf.next_leaf = right_leaf.page->get_page_number();
        *next_leaf = middle_leaf.page->get_page_number();

        // middle left
        uint64_t n_records_middle = (*value_count + 1) - (n_records_right + n_records_left);
        unsigned char* middle_reference = buffer + n_records_left * (N * 8);

        std::bitset<N * 8> middle_bitset;
        middle_bitset.set();
        for (uint64_t i = 0; i < n_records_middle; ++i) {
            unsigned char* current_record = buffer + (n_records_left + i) * (N * 8);
            for (size_t j = 0; j < N * 8; ++j) {
                if (middle_bitset[j] && current_record[j] != middle_reference[j]) {
                    middle_bitset.set(i, 0);
                }
            }
        }

        update_leaf(middle_leaf, middle_bitset, n_records_middle, buffer + n_records_left * (N * 8));

        // left leaf
        update_leaf(*this, left_bitset, n_records_left, buffer);

        // right leaf
        update_leaf(right_leaf, right_bitset, n_records_right, buffer + (n_records_left + n_records_middle) * (N * 8));

        const auto middle_first = middle_leaf.get_record(0);
        const auto right_first = right_leaf.get_record(0);
        return std::make_unique<BPlusTreeSplit<N>>(middle_first,
                                                   middle_page.get_page_number(),
                                                   right_first,
                                                   right_page.get_page_number());
    }
}


// returns the position of the minimum key greater or equal than the record given.
// if there is no such key, returns (to + 1)
template <std::size_t N>
uint_fast32_t BPlusTreeLeaf<N>::search_index(const Record<N>& record) const noexcept {
    int_fast32_t from = 0;
    int_fast32_t to = static_cast<int_fast32_t>(*value_count)-1;
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


template <std::size_t N>
void BPlusTreeLeaf<N>::shift_right_records(int_fast32_t from, int_fast32_t to) {
    uint64_t record_step = N * 8 - redundant_count;

    for (auto i = to; i >= from; i--) {
        for (size_t j = 0; j < record_step; j++) {
            records[(i + 1) * record_step + j] = records[i * record_step + j];
        }
    }
}


template <std::size_t N>
bool BPlusTreeLeaf<N>::equal_record(const Record<N>& record, uint_fast32_t index) {
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


template <std::size_t N>
bool BPlusTreeLeaf<N>::check_range(const Record<N>& r) const {
    if (*value_count == 0) {
        return false;
    }
    auto min = get_record(0);
    auto max = get_record(*value_count - 1);

    return min <= r && r <= max;
}


template <std::size_t N>
void BPlusTreeLeaf<N>::print(std::ostream& os) const {
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
            }
            else {
                os << current_record[pos_unique];
                pos_unique++;
            }
        }
        os << ")\n";
    }
}


template <std::size_t N>
bool BPlusTreeLeaf<N>::check(std::ostream& os) const {
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
