#include "trie.h"

#include <cassert>
#include <iostream>

#include "storage/buffer_manager.h"
#include "storage/file_manager.h"
#include "storage/page.h"

Trie::Trie() :
    node_file_id (file_manager.get_file_id("text_search.node")),
    info_file_id (file_manager.get_file_id("text_search.info")),
    root (TrieNode(*this, buffer_manager.get_page(node_file_id, 0), root_node_offset))
{
    // Get first page of node_file_id
    auto& page = buffer_manager.get_page(node_file_id, 0);

    // Read last offsets
    last_offset_node_page = reinterpret_cast<uint64_t*> (page.get_bytes());
    last_offset_info_page = reinterpret_cast<uint64_t*> (page.get_bytes() + 8);

    // Check if last_offset_node_page is not valid (Root node must always exist)
    if (*last_offset_node_page == 0) {
        *last_offset_node_page = root_node_offset + NODE_SIZE; // Space of both last_offsets and root node
        page.make_dirty();
    }

    buffer_manager.unpin(page);
}


void Trie::add_new_word(const std::string& str, uint64_t id) {
    root.insert(reinterpret_cast<const unsigned char*> (str.data()), id);
}


TrieIter Trie::get_iter(const std::string& str) {
    return root.get_iter(reinterpret_cast<const unsigned char*> (str.data()), false);
}


TrieIter Trie::get_exact_iter(const std::string& str) {
    return root.get_iter(reinterpret_cast<const unsigned char*> (str.data()), true);
}


TrieErrorIter Trie::get_iter_error_search(const std::string& str, int max_errors) {
    TrieErrorIter iter(false);
    iter.begin(reinterpret_cast<const unsigned char*> (str.data()), root.clone(), max_errors);

    return iter;
}


TrieErrorIter Trie::get_exact_iter_error_search(const std::string& str, int max_errors) {
    TrieErrorIter iter(true);
    iter.begin(reinterpret_cast<const unsigned char*> (str.data()), root.clone(), max_errors);

    return iter;
}


void Trie::print_trie() {
    root.print_trie_node();
    std::cout << '\n';
}


void Trie::delete_word(const std::string& str) {
    root.deletion(reinterpret_cast<const unsigned char*> (str.data()));
}
