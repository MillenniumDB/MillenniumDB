#pragma once

#include <stack>
#include <memory>

#include "storage/buffer_manager.h"
#include "storage/index/bplus_tree/bplus_tree_leaf.h"
#include "storage/index/bplus_tree/bplus_tree_split.h"
#include "storage/index/record.h"
#include "storage/page.h"

template <std::size_t N> class BPlusTree;

template <std::size_t N>
class BPlusTreeDir {
friend class BPlusTree<N>;

public:
    BPlusTreeDir(FileId leaf_file_id, Page& page) :
        keys         (reinterpret_cast<uint64_t*>(page.get_bytes())),
        key_count    (reinterpret_cast<uint32_t*>(page.get_bytes()
                        + (sizeof(uint64_t) * BPlusTree<N>::dir_max_records * N))),
        children     (reinterpret_cast<int32_t*>(page.get_bytes()
                        + (sizeof(uint64_t) * BPlusTree<N>::dir_max_records * N)
                        + sizeof(uint32_t))),
        page         (page),
        dir_file_id  (page.page_id.file_id),
        leaf_file_id (leaf_file_id) { }


    ~BPlusTreeDir() {
        buffer_manager.unpin(page);
    }

    std::unique_ptr<BPlusTreeSplit<N>> bulk_insert(BPlusTreeLeaf<N>& leaf);

    // returns not null when it needs to split
    std::unique_ptr<BPlusTreeSplit<N>> insert(const Record<N>& record);

    // returns a leaf and the position of the first record r >= min.
    // If there is no such record the position returned is at the end of the leaf
    SearchLeafResult<N> search_leaf(const Record<N>& min) const noexcept;

    // same as previous search_leaf but the BPlusTreeDir branch is added to the stack
    SearchLeafResult<N> search_leaf(std::stack< std::unique_ptr<BPlusTreeDir<N>> >&,
                                    const Record<N>& min) const noexcept;

    // returns true if min_key <= r <= max_key. If key_count==0, will return false.
    // used in leapfrog to know if the search can be done from here or from a upper directory in the branch
    bool check_range(const Record<N>& r) const;

    // returns false if an error in this directory is found
    bool check() const;

private:
    uint64_t* const keys;
    uint32_t* const key_count;
    int32_t* const children;

    Page& page;
    const FileId dir_file_id;
    const FileId leaf_file_id;

    size_t search_child_index(const Record<N>& record) const noexcept;
    void shift_right_keys(int_fast32_t from, int_fast32_t to);
    void shift_right_children(int_fast32_t from, int_fast32_t to);
    void update_key(int_fast32_t index, const Record<N>& record);
    void update_child(int_fast32_t index, int_fast32_t dir);
    void split(const Record<N>& record);
};
