#pragma once

#include <memory>

#include "storage/buffer_manager.h"
#include "storage/index/bplus_tree/bplus_tree_split.h"
#include "storage/index/record.h"
#include "storage/page.h"

// forward declarations
template <std::size_t N> class BPlusTreeDir;
template <std::size_t N> class BPlusTreeLeaf;
template <std::size_t N> class BPlusTree;


template <std::size_t N>
struct SearchLeafResult {
    std::unique_ptr<BPlusTreeLeaf<N>> leaf;
    uint_fast32_t result_index;

    SearchLeafResult(std::unique_ptr<BPlusTreeLeaf<N>> leaf, uint_fast32_t result_index) :
        leaf         (std::move(leaf)),
        result_index (result_index) { }
};


template <std::size_t N>
class BPlusTreeLeaf {
friend class BPlusTreeDir<N>;
friend class BPlusTree<N>;

public:
    BPlusTreeLeaf(Page& page) :
        records      ( reinterpret_cast<uint64_t*>(page.get_bytes() + (2*sizeof(uint32_t)) ) ),
        value_count  ( reinterpret_cast<uint32_t*>(page.get_bytes()) ),
        next_leaf    ( reinterpret_cast<uint32_t*>(page.get_bytes() + sizeof(uint32_t)) ),
        page         (page),
        leaf_file_id (page.page_id.file_id) { }

    ~BPlusTreeLeaf() {
        buffer_manager.unpin(page);
    }

    Page& get_page()           const noexcept { return page; }
    uint32_t get_value_count() const { return *value_count; }
    bool has_next()            const { return *next_leaf != 0; }

    // returns false if an error in this leaf is found
    bool check() const;

    // only for debugging
    void print() const;

    std::unique_ptr<BPlusTreeSplit<N>> insert(const Record<N>& record);

    std::unique_ptr<BPlusTreeLeaf<N>> duplicate() const {
        buffer_manager.pin(page);
        return std::make_unique<BPlusTreeLeaf<N>>(page);
    }

    std::unique_ptr<BPlusTreeLeaf<N>> get_next_leaf() const {
        Page& new_page = buffer_manager.get_page(leaf_file_id, *next_leaf);
        return std::make_unique<BPlusTreeLeaf<N>>(new_page);
    }

    std::unique_ptr<Record<N>> get_record(uint_fast32_t pos) const; // asumes pos is valid

    // Search for the first record that is equal or greater than the parameter recived.
    // May give an invalid index, meaning there is no such record is on this page.
    // If the next leaf is not null, the desired record should be the first record of that leaf,
    // otherwise the record is not in the B+tree.
    uint_fast32_t search_index(const Record<N>& record) const noexcept;

    // returns true if min_record <= r <= max_record. If the leaf is empty will return false.
    // used in leapfrog to know if the search can be done from here or from a upper directory in the branch
    bool check_range(const Record<N>& r) const;

private:
    uint64_t* const records;
    uint32_t* const value_count;
    uint32_t* const next_leaf;

    Page& page;
    const FileId leaf_file_id;

    bool equal_record(const Record<N>& record, uint_fast32_t index);
    void shift_right_records(int_fast32_t from, int_fast32_t to);
};
