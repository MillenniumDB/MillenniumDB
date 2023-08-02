#pragma once

#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <queue>

#include "storage/index/text_search/trie_iter.h"
#include "storage/index/text_search/trie_error_iter.h"
#include "storage/index/text_search/trie_node.h"
#include "storage/index/text_search/trie_garbage.h"
#include "storage/file_id.h"
#include "storage/page.h"


class Trie {
public:

    // File ids for FileManager
    const FileId node_file_id;
    const FileId info_file_id;

    // Garbage Collector
    TrieGarbage garbage;

    // Definition of constants
    static constexpr uint64_t NODE_SIZE = TrieNode::TOTAL_NODE_SIZE; // Node size in node_page (11)
    static constexpr uint64_t PAGE_SIZE = Page::MDB_PAGE_SIZE; // Page size (Normally 4096B)
    static constexpr uint64_t CAPACITY = 16; /// Initial capacity for info_page
    static constexpr uint64_t root_node_offset = 16; // First 16B are for last_offset_node_page and last_offset_info_page

    // Pointers to offsets to write in files
    uint64_t* last_offset_node_page;    // 8B
    uint64_t* last_offset_info_page;    // 8B
    // Total 16B => root_node_offset

    Trie();

    TrieNode root;

    // Adds a new word to Trie
    void add_new_word(const std::string& str, uint64_t id);

    // Prefix search in Trie. Returns TrieIter.
    TrieIter get_iter(const std::string& str);

    // Exact search in Trie. Returns TrieIter.
    TrieIter get_exact_iter(const std::string& str);

    // Prefix search in Trie for word matches with up to max_errors errors
    TrieErrorIter get_iter_error_search(const std::string& str, int max_errors = 3);

    // Exact search in Trie for word matches with up to max_errors errors
    TrieErrorIter get_exact_iter_error_search(const std::string& str, int max_errors = 3);

    // Prints Trie to console (Debug)
    void print_trie();

    // Deletes a word from Trie
    void delete_word(const std::string& str);
};