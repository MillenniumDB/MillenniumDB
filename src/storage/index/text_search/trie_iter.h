#pragma once

#include <cstdint>

namespace TextSearch {

// Pure virtual TrieIter that defines the interface
// of the iterators returned by the trie searches.
class TrieIter {
public:
    // Returns true if a node was found
    virtual bool next() = 0;

    // Called when next() returns true to get the node_id of the current node
    virtual uint64_t get_node_id() = 0;

    virtual ~TrieIter() = default;
};

} // namespace TextSearch
