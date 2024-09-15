#pragma once

#include <cstdint>
#include <cstring>
#include <memory>
#include <string>
#include <vector>

#include "storage/file_id.h"
#include "storage/page/unversioned_page.h"
#include "storage/index/text_search/search_type.h"

namespace TextSearch {

class Trie;

class Node {
    friend class TrieIterList;
    template<SearchType, bool> friend class TrieIterSearch;

public:
    // Initialize pre-existing node
    Node(Trie& trie, uint64_t page_pointer);
    Node(Trie& trie, UPage* page, uint64_t page_offset);

    // Create new node with string of length <new_string> and capacity for <child_count> children, and possibly a pointer to <child_node>
    Node(Trie& trie, const unsigned char* new_string, size_t string_length, size_t child_count, Node* child_node, unsigned char child_char);

    ~Node();

    Node(const Node&) = delete;
    Node(Node&&) = delete;
    Node& operator=(const Node& other) = delete;
    Node& operator=(Node&& other) = delete;

    // Duplicates TrieNode
    std::unique_ptr<Node> clone();

    // Recursive function to insert a string in a node
    // parent_child_page_pointer_ptr: ptr to page_pointer to child of parent
    uint64_t insert_string(Node* parent, unsigned char* parent_child_page_pointer_ptr, const unsigned char* str);

    // Returns the pointer to this node (combination of page_number and page_offset)
    uint64_t page_pointer() {
        return page->get_page_number() * UPage::SIZE + page_offset;
    }

    // Print tire in dot format
    void print_trie_node(std::ostream& os);

private:
    Trie& trie;

    UPage* page;
    uint64_t page_offset;


    // The following are pointers to all the node contents
    unsigned char* node_id_ptr;        // 5B
    unsigned char* capacity_ptr;       // 2B
    unsigned char* document_count_ptr; // 1B
    unsigned char* str_len_ptr;        // 1B
    unsigned char* child_count_ptr;    // 1B
    unsigned char* string_ptr;         // (1B char) * string_length
    unsigned char* children_ptr;       // (1B char, 5B Page Pointer) * child_count


    // Constants used to calculate the pointer positions
    static constexpr size_t NODE_ID_SIZE        = 5;
    static constexpr size_t CAPACITY_SIZE       = 2;
    static constexpr size_t DOCUMENT_COUNT_SIZE = 1;
    static constexpr size_t DOCUMENT_COUNT_MAX  = 0xff;
    static constexpr size_t STR_LEN_SIZE        = 1;
    static constexpr size_t STR_LEN_MAX         = 0xff;
    static constexpr size_t CHILD_COUNT_SIZE    = 1;
    static constexpr size_t CHILD_CHAR_SIZE     = 1;
    static constexpr size_t CHILD_POINTER_SIZE  = 5;
    static constexpr size_t CHILD_SIZE = CHILD_CHAR_SIZE + CHILD_POINTER_SIZE;


    std::unique_ptr<Node> insert_child(Node* parent, unsigned char* parent_child_page_pointer_ptr, const unsigned char* string);

    // Calculates spaces needed for this Node
    size_t size();
    // Calculates space needed for a node with string_length and child_count
    size_t size(size_t string_length, size_t child_count);

    // Increments the document count at the current node.
    // Mainly used for debugging, and potentially be removed.
    // Instead we could rely on whether the node_id of the trie
    // node is present in the BPT.
    void increment_document_count();

    // Initialize pointers of an existing Node using the counts in the Page
    void init_pointers_using_page();

    // Initialize pointers of a new Node using the counts passed as arguments
    void init_pointers_using_string_length(size_t string_length);

    // Split this node, inserting a new node between parent and this node.
    // parent_child_page_pointer_ptr: ptr to page_pointer to child of parent
    // split_position indicates where to split.
    // For example:
    // - split_position=0: "abc" -> "", "abc"
    // - split_position=1: "abc" -> "a", "bc"
    // Returns the newly created node.
    std::unique_ptr<Node> split(Node* parent, unsigned char* parent_child_page_pointer_ptr, size_t split_position);

    // Gets new space of size >= capacity. Checks the trie garbage first.
    // The capacity is rounded up the next power of 2.
    std::pair<uint64_t, uint64_t> get_space(uint64_t& capacity);
};

} // namespace TextSearch
