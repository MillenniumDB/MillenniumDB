#pragma once

#include <memory>

#include "storage/index/bplus_tree/bplus_tree.h"
#include "storage/index/text_search/table.h"
#include "storage/index/text_search/text_search_iter.h"
#include "storage/index/text_search/trie_iter.h"

namespace TextSearch {

// The IndexIter is returned when making a search and permits iterating over the results
class IndexIter : public TextSearchIter{
public:
    IndexIter(std::unique_ptr<TrieIter> trie_iter, BPlusTree<2>& bpt) :
        trie_iter(std::move(trie_iter)), bpt(bpt) { }

    // Returns true if there is a result
    bool next() override;

    // Returns the table pointer associated with the current result
    uint64_t get_table_pointer() const override;

private:
    // Iterator that makes the search in the trie
    std::unique_ptr<TrieIter> trie_iter;
    // The BPlusTree used to obtain table pointers from the trie node IDs
    BPlusTree<2>&             bpt;
    // The table containing the data added during indexing

    // Iterator for a specific trie node_id that was returned by the trie_tier
    std::unique_ptr<BptIter<2>> bpt_iter;

    uint64_t table_pointer;

    // Only needed to pass to bpt.get_range()
    bool interruption_requested = false;
};

} // namespace TextSearch
