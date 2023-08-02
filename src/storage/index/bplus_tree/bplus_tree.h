#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <string>

#include "storage/file_id.h"
#include "storage/index/bplus_tree/bplus_tree_dir.h"
#include "storage/index/bplus_tree/bplus_tree_leaf.h"
#include "storage/index/record.h"

template <std::size_t N> class OrderedFile;

template <std::size_t N> class BptIter {
public:
    // shouldn't use a BptIter constructed like this.
    // This exists only to allow reserving space and then reassign to a valid BptIter
    BptIter() noexcept :
        interruption_requested (nullptr) { }

    BptIter(bool* interruption_requested,
            SearchLeafResult<N>&& leaf_and_pos,
            const Record<N>& max) noexcept :
        interruption_requested (interruption_requested),
        current_pos            (leaf_and_pos.result_index),
        max                    (max),
        current_leaf           (std::move(leaf_and_pos.leaf)) { }

    BptIter(BptIter&& other) noexcept :
        interruption_requested (other.interruption_requested),
        current_pos            (other.current_pos),
        max                    (std::move(other.max)),
        current_leaf           (std::move(other.current_leaf)) { }

    void operator=(BptIter&& other) noexcept {
        interruption_requested = other.interruption_requested;
        current_pos            = other.current_pos;
        max                    = std::move(other.max);
        current_leaf           = std::move(other.current_leaf);
    }

    const Record<N>* next();

    inline bool is_null() const {
        return interruption_requested == nullptr;
    }

    inline void set_null() {
        this->interruption_requested = nullptr;
        // current_leaf = BPlusTreeLeaf<N>();
    }

private:
    bool* interruption_requested;
    uint_fast32_t current_pos;
    Record<N> current_record;
    Record<N> max;
    BPlusTreeLeaf<N> current_leaf;
};


template <std::size_t N> class BPlusTree {
public:
    // (MDB_PAGE_SIZE - SIZE_OF(value_count) - SIZE_OF(next_leaf)) / (SIZE_OF(UINT64) * N)
    static constexpr auto leaf_max_records = (Page::MDB_PAGE_SIZE - 2*sizeof(int32_t) ) / (sizeof(uint64_t)*N);
    static constexpr auto dir_max_records  = (Page::MDB_PAGE_SIZE - 2*sizeof(int32_t) ) / (sizeof(uint64_t)*N + sizeof(int32_t));

    BPlusTree(const std::string& name);

    const FileId dir_file_id;
    const FileId leaf_file_id;

    // void bulk_import(OrderedFile<N>&);
    void insert(const Record<N>& record);

    // returns false if an error in the BPT is found
    bool check() const;

    BptIter<N> get_range(bool* interruption_requested,
                         const Record<N>& min,
                         const Record<N>& max) const noexcept;

    // It doesn't simply return the root, it is an unique_ptr so it pins the page
    std::unique_ptr<BPlusTreeDir<N>> get_root() const noexcept;

private:
    BPlusTreeDir<N> root;
};
