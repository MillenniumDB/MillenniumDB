#pragma once

#include <memory>
#include <utility>

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
    BPlusTreeLeaf<N> leaf;
    uint_fast32_t result_index;

    SearchLeafResult(BPlusTreeLeaf<N>&& leaf, uint_fast32_t result_index) noexcept :
        leaf         (std::move(leaf)),
        result_index (result_index) { }
};


template <std::size_t N>
class BPlusTreeLeaf {

friend class BPlusTreeDir<N>;
friend class BPlusTree<N>;

public:
    BPlusTreeLeaf() noexcept :
        page         (nullptr),
        leaf_file_id (FileId::UNASSIGNED) { }

    BPlusTreeLeaf(Page* page) noexcept :
        records      ( reinterpret_cast<uint64_t*>(page->get_bytes() + (2*sizeof(uint32_t)) ) ),
        value_count  ( reinterpret_cast<uint32_t*>(page->get_bytes()) ),
        next_leaf    ( reinterpret_cast<uint32_t*>(page->get_bytes() + sizeof(uint32_t)) ),
        page         (page),
        leaf_file_id (page->page_id.file_id) { }

    ~BPlusTreeLeaf() {
        if (page != nullptr)
            buffer_manager.unpin(*page);
    }

    BPlusTreeLeaf(BPlusTreeLeaf&& other) noexcept :
        records      (other.records),
        value_count  (other.value_count),
        next_leaf    (other.next_leaf),
        page         (std::exchange(other.page, nullptr)),
        leaf_file_id (other.leaf_file_id) { }

    void operator=(BPlusTreeLeaf&& other) noexcept {
        records      = other.records;
        value_count  = other.value_count;
        next_leaf    = other.next_leaf;
        leaf_file_id = other.leaf_file_id;

        this->page = std::exchange(other.page, this->page);
    }

    inline Page& get_page()           const { return *page; }
    inline uint32_t get_value_count() const { return *value_count; }
    inline bool has_next()            const { return *next_leaf != 0; }

    // returns false if an error in this leaf is found
    bool check() const;

    // only for debugging
    void print() const;

    std::unique_ptr<BPlusTreeSplit<N>> insert(const Record<N>& record);

    BPlusTreeLeaf<N> clone() const {
        buffer_manager.pin(*page);
        return BPlusTreeLeaf<N>(page);
    }

    void update_to_next_leaf() {
        buffer_manager.unpin(*page);

        this->page        = &buffer_manager.get_page(leaf_file_id, *next_leaf);
        this->records     = reinterpret_cast<uint64_t*>(page->get_bytes() + (2*sizeof(uint32_t)) );
        this->value_count = reinterpret_cast<uint32_t*>(page->get_bytes());
        this->next_leaf   = reinterpret_cast<uint32_t*>(page->get_bytes() + sizeof(uint32_t));
    }

    // Writes a record in a given space
    // assumes pos is valid
    void get_record(uint_fast32_t pos, Record<N>* out) const;

    // Search for the first record that is equal or greater than the parameter received.
    // May give an invalid index, meaning there is no such record is on this page.
    // If the next leaf is not null, the desired record should be the first record of that leaf,
    // otherwise the record is not in the B+tree.
    uint_fast32_t search_index(const Record<N>& record) const noexcept;

    // returns true if min_record <= r <= max_record. If the leaf is empty will return false.
    // used in leapfrog to know if the search can be done from here or from a upper directory in the branch
    bool check_range(const Record<N>& r) const;

private:
    uint64_t* records;
    uint32_t* value_count;
    uint32_t* next_leaf;

    Page* page;
    FileId leaf_file_id;

    bool equal_record(const Record<N>& record, uint_fast32_t index);
    void shift_right_records(int_fast32_t from, int_fast32_t to);
};
