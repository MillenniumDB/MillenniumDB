#ifndef STORAGE__ORDERED_FILE_PAGE_H_
#define STORAGE__ORDERED_FILE_PAGE_H_

#include "storage/index/record.h"
#include "storage/index/bplus_tree/bplus_tree.h"
#include "storage/page.h"

template <std::size_t N>
class OrderedFilePage {
public:
    static constexpr auto MAX_RECORDS = BPlusTree<N>::leaf_max_records;

    static_assert(
        MAX_RECORDS*N*sizeof(uint64_t) + sizeof(uint32_t) + sizeof(uint32_t) <= Page::MDB_PAGE_SIZE
    );

    OrderedFilePage(Page& page) noexcept;
    ~OrderedFilePage();

    inline bool is_full() const noexcept {
        return *size >= MAX_RECORDS;
    }

    inline void clear() noexcept { *size = 0; }
    inline uint32_t get_size() const noexcept { return *size; }
    inline Page& get_page() const noexcept { return page; }

    void append_record(const std::array<uint64_t, N>& record) noexcept;
    void reorder_columns(const std::array<uint_fast8_t, N>& column_order) noexcept;
    void order() noexcept;

    std::array<uint64_t, N> get(const uint_fast32_t pos) const noexcept;

private:
    Page& page;

    uint32_t* const size;
    uint64_t* const records;
};

#endif // STORAGE__ORDERED_FILE_PAGE_H_
