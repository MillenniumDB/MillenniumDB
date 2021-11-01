/*
 * OrderedFile allows inserting a large number of records and then sort them in
 * ascending order. It also allows sorting by changing the order of the columns,
 * which is useful for creating different B+Tree permutations.
 * The ordering is perfomed using blocks, each block will be ordered using
 * insertion sort and then will be merged until the complete file is ordered.
 * Merging phase uses a temporary file to store the result.
 * */

#ifndef STORAGE__ORDERED_FILE_H_
#define STORAGE__ORDERED_FILE_H_

#include <memory>
#include <string>

#include "storage/file_id.h"
#include "storage/index/bplus_tree/bplus_tree.h"
#include "storage/index/record.h"
#include "storage/index/ordered_file/ordered_file_page.h"

template <std::size_t N>
class OrderedFile {
public:
    static_assert(N <= UINT8_MAX);

    OrderedFile(const std::string& filename);
    ~OrderedFile();

    void append_record(const std::array<uint64_t, N>& record) noexcept;
    void order(const std::array<uint_fast8_t, N>& column_order) noexcept;

    void begin_read() noexcept;
    void copy_to_bpt_leaf(BPlusTreeLeaf<N>& leaf, uint_fast32_t page_number) const noexcept;

    // next_record() is used to get all the records one by one. begin_read() must be called at first.
    // example:
    // ordered_file.begin_read();
    // for (auto record = ordered_file.next_record(); record != nullptr; record = ordered_file.next_record()) {
    //     // do something with record
    // }
    std::unique_ptr<Record<N>> next_record() noexcept;

    inline uint_fast32_t get_last_page() const noexcept { return last_page; }

    // Only used to debug purposes
    bool check_order();

private:
    const FileId file_id_a;
    const FileId file_id_b;

    const FileId* file_id;
    const FileId* helper_file_id;

    uint_fast32_t current_page;
    uint_fast32_t current_pos_in_current_page;

    uint_fast32_t last_page;
    uint_fast32_t last_pos_in_last_page;

    // used in append_record and next_record
    std::unique_ptr<OrderedFilePage<N>> io_buffer;
};

#endif // STORAGE__ORDERED_FILE_H_
