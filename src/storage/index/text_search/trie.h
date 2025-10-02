#pragma once

#include <cstdint>
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include "storage/file_id.h"
#include "storage/index/text_search/search_type.h"
#include "storage/index/text_search/trie_garbage.h"
#include "storage/index/text_search/trie_iter.h"
#include "storage/index/text_search/trie_iter_list.h"

namespace TextSearch {

// First 8 bytes of file_id has the end_page_pointer
// Next 8 bytes has the id_count (counter to generate unique ids for nodes)
// Next bytes is the root node
class Trie {
public:
    // Initial capacity for non-root nodes
    static constexpr uint64_t INITIAL_CAPACITY = 16;

    static constexpr uint64_t ROOT_ID = 16;

    // FileId of the file containing the trie.
    // All the nodes are in one file.
    const FileId file_id;

    // Garbage Collector
    TrieGarbage garbage;

    Trie(const std::filesystem::path& path);

    // Adds a new word to Trie
    uint64_t insert_string(const std::string& str);

    uint64_t get_new_id();

    // Iterator over all the strings that have been inserted into the trie
    TrieIterList get_iter_list();

    // Gets new space of size >= capacity. Checks the trie garbage first.
    // The capacity is rounded up the next power of 2.
    std::pair<uint64_t, uint64_t> get_space(uint64_t& capacity);

    // Search in the trie
    template<SearchType type, bool allow_errors>
    std::unique_ptr<TrieIter> search(const std::string& query);

    // Prints the trie to os in DOT format (Graphviz)
    void print_trie(std::ostream& os, std::vector<std::string>&& text_list);
};

} // namespace TextSearch
