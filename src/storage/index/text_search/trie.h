#pragma once

#include <cstdint>
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include "storage/file_id.h"
#include "storage/index/text_search/trie_garbage.h"
#include "storage/index/text_search/trie_iter_list.h"
#include "storage/index/text_search/trie_iter_search.h"
#include "storage/index/text_search/trie_node.h"

namespace TextSearch {

class Trie {
    friend class Node;

public:
    static std::unique_ptr<Trie> create(const std::filesystem::path& path);

    static std::unique_ptr<Trie> load(const std::filesystem::path& path);

    ~Trie();

    // Adds a new word to Trie
    uint64_t insert_string(const std::string& str);

    // Iterator over all the strings that have been inserted into the trie
    TrieIterList get_iter_list();

    // Search in the trie
    template<SearchType type, bool allow_errors>
    std::unique_ptr<TrieIter> search(const std::string& query);

    // Prints the trie to os in DOT format (Graphviz)
    void print_trie(std::ostream& os, std::vector<std::string>&& text_list);

private:
    explicit Trie(FileId file_id, UPage& root_page, std::unique_ptr<TrieGarbage> garbage, bool load);

    // FileId of the file containing the trie.
    // All the nodes are in one file.
    const FileId file_id;

    // Definition of constants
    static constexpr uint64_t CAPACITY = 16; // Initial capacity for nodes
    static constexpr uint64_t PAGE_POINTER_SIZE = 5;
    static constexpr uint64_t NEXT_ID_SIZE = 5;
    static constexpr uint64_t HEADER_SIZE = 2 * PAGE_POINTER_SIZE + NEXT_ID_SIZE;

    // Pointers to offsets to write in files
    unsigned char* end_page_pointer_ptr;  // ptr to the page pointer to the start of unused space, 5B
    unsigned char* root_page_pointer_ptr; // ptr to the page pointer to the root node, 5B
    unsigned char* next_id_ptr;           // ptr to the next id to use for nodes, 5B

    std::unique_ptr<Node> root_node;
    UPage& root_page;

    // Garbage Collector
    std::unique_ptr<TrieGarbage> garbage;
};

} // namespace TextSearch
