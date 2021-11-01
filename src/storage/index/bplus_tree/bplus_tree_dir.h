#ifndef STORAGE__INDEX__B_PLUS_TREE_DIR_H_
#define STORAGE__INDEX__B_PLUS_TREE_DIR_H_

#include <stack>
#include <memory>

#include "storage/index/bplus_tree/bplus_tree_leaf.h"
#include "storage/index/bplus_tree/bplus_tree_split.h"
#include "storage/index/record.h"
#include "storage/page.h"

template <std::size_t N> class BPlusTree;

template <std::size_t N>
class BPlusTreeDir {
friend class BPlusTree<N>;

public:
    BPlusTreeDir(FileId const leaf_file_id, Page& page);
    ~BPlusTreeDir();

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
    FileId const dir_file_id;
    FileId const leaf_file_id;
    Page& page;
    uint64_t* const keys;
    uint32_t* const key_count;
    int32_t* const children;

    int search_child_index(int from, int to, const Record<N>& record) const;
    void shift_right_keys(int from, int to);
    void shift_right_children(int from, int to);
    void update_key(int index, const Record<N>& record);
    void update_child(int index, int dir);
    void split(const Record<N>& record);
};

#endif // STORAGE__INDEX__B_PLUS_TREE_DIR_H_
