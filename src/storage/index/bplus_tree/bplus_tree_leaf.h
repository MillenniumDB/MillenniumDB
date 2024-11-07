#pragma once

#include <bitset>
#include <memory>
#include <ostream>
#include <utility>

#include "storage/index/bplus_tree/bplus_tree_split.h"
#include "storage/index/record.h"
#include "storage/page/versioned_page.h"

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

    BPlusTreeLeaf(VPage* page) noexcept :
        value_count  ( reinterpret_cast<uint32_t*>(page->get_bytes())),
        next_leaf    ( reinterpret_cast<uint32_t*>(page->get_bytes() + sizeof(uint32_t))),
        bitset_ptr   ((unsigned char*) page->get_bytes() + 2 * sizeof(uint32_t)),
        redundant_bytes((unsigned char*) page->get_bytes() + 2 * sizeof(uint32_t) + N),
        page         (page),
        leaf_file_id (page->page_id.file_id) {
        char* bitset_ptr = page->get_bytes() + 2 * sizeof(uint32_t);

        int pos_bitset = 0;
        for (size_t i = 0; i < N; ++i) {
            for (int bit = 0; bit < 8; bit++) {
                redundant_bitset.set(pos_bitset++, (bitset_ptr[i] >> bit) & 1);
            }
        }
        redundant_count = redundant_bitset.count();
        records = (unsigned char*) page->get_bytes() + 2 * sizeof(uint32_t) + N + redundant_count;
    }

    BPlusTreeLeaf(BPlusTreeLeaf&& other) noexcept :
        records      (other.records),
        value_count  (other.value_count),
        next_leaf    (other.next_leaf),
        bitset_ptr   (other.bitset_ptr),
        redundant_bytes(other.redundant_bytes),
        redundant_count(other.redundant_count),
        redundant_bitset(other.redundant_bitset),
        page         (std::exchange(other.page, nullptr)),
        leaf_file_id (other.leaf_file_id) { }

    ~BPlusTreeLeaf();

    void operator=(BPlusTreeLeaf&& other) noexcept {
        records      = other.records;
        value_count  = other.value_count;
        next_leaf    = other.next_leaf;
        bitset_ptr   = other.bitset_ptr;
        leaf_file_id = other.leaf_file_id;
        redundant_bytes = other.redundant_bytes;
        redundant_count = other.redundant_count;
        redundant_bitset = other.redundant_bitset;

        this->page = std::exchange(other.page, this->page);
    }

    BPlusTreeLeaf<N> clone() const;

    void update_to_next_leaf();

    inline VPage& get_page()          const { return *page; }
    inline uint32_t get_value_count() const { return *value_count; }
    inline bool has_next()            const { return *next_leaf != 0; }

    // returns false if an error in this leaf is found
    bool check(std::ostream& os) const;

    // only for debugging
    void print(std::ostream& os) const;

    std::unique_ptr<BPlusTreeSplit<N>> insert(const Record<N>& record, bool& error);

    // returns true if record was deleted, false if record did not exists
    bool delete_record(const Record<N>& record);

    // Writes a record in a given space
    // assumes pos is valid
    void set_record(uint_fast32_t pos, Record<N>& out) const;

    // Initializes a record and returns it
    Record<N> get_record(uint_fast32_t pos) const;

    // Sets the redundant bytes in out
    void set_redundant_record(Record<N>& out) const;

    // Updates out with the non redundant bytes
    void update_record(uint_fast32_t pos, Record<N>& out) const;

    // Search for the first record that is equal or greater than the parameter received.
    // May give an invalid index, meaning there is no such record is on this page.
    // If the next leaf is not null, the desired record should be the first record of that leaf,
    // otherwise the record is not in the B+tree.
    uint_fast32_t search_index(const Record<N>& record) const noexcept;

    // returns true if min_record <= r <= max_record. If the leaf is empty will return false.
    // used in leapfrog to know if the search can be done from here or from a upper directory in the branch
    bool check_range(const Record<N>& r) const;

private:
    unsigned char* records;
    uint32_t* value_count;
    uint32_t* next_leaf;
    unsigned char* bitset_ptr;
    unsigned char* redundant_bytes;
    uint32_t redundant_count = 0;

    // The bits set to true represent the byte position of the records that are redundant.
    std::bitset<N * 8> redundant_bitset;

    VPage* page;
    FileId leaf_file_id;

    uint32_t get_page_size(std::bitset<N * 8> bitset, uint32_t n_records);
    std::bitset<N * 8> create_new_bitset(const Record<N>& reference, uint64_t from, uint64_t to);

    void update_leaf(BPlusTreeLeaf<N>& leaf, std::bitset<N * 8>& bitset, uint64_t n_records, unsigned char* buffer);
    void compress_to_buffer(unsigned char* compression_buffer, std::bitset<N * 8> bitset, uint64_t from, uint64_t to);
    void upgrade_to_editable();

    bool equal_record(const Record<N>& record, uint_fast32_t index);
    void shift_right_records(int_fast32_t from, int_fast32_t to);
};
