#pragma once

#include <cstdint>
#include <cstring>
#include <memory>

#include "storage/index/text_search/search_type.h"
#include "storage/page/page.h"

namespace TextSearch {

class Trie;

class Node {
    friend class Trie;
    friend class TrieIterList;

    template<SearchType, bool>
    friend class TrieIterSearch;

public:
    // Constants used to calculate the pointer positions
    static constexpr size_t NODE_ID_SIZE = 5;
    static constexpr size_t CAPACITY_SIZE = 2;
    static constexpr size_t DOCUMENT_COUNT_SIZE = 1;
    static constexpr size_t DOCUMENT_COUNT_MAX = 0xFF;
    static constexpr size_t STR_LEN_SIZE = 1;
    static constexpr size_t STR_LEN_MAX = 0xFF;
    static constexpr size_t CHILD_COUNT_SIZE = 1;
    static constexpr size_t CHILD_CHAR_SIZE = 1;
    static constexpr size_t CHILD_POINTER_SIZE = 5;
    static constexpr size_t CHILD_SIZE = CHILD_CHAR_SIZE + CHILD_POINTER_SIZE;
    static constexpr size_t MAX_NODE_SIZE = NODE_ID_SIZE + CAPACITY_SIZE + DOCUMENT_COUNT_SIZE + STR_LEN_SIZE
                                          + STR_LEN_MAX + CHILD_COUNT_SIZE + (CHILD_SIZE * 0xFF);

    // Initialize pre-existing node
    // node pos is the absolute position in the file
    Node(Trie& trie, uint64_t node_pos);

    // receives a pinned page
    Node(Trie& trie, Page* page, uint64_t page_offset);

    // Create new node with string of length <new_string> and capacity for <child_count> children, and possibly a pointer to <child_node>
    Node(
        Trie& trie,
        const uint8_t* new_string,
        size_t string_length,
        size_t child_count,
        Node* child_node,
        unsigned char child_char
    );

    static void init_root(Trie& trie, Page* page, uint64_t page_offset);

    ~Node();

    Node(const Node&) = delete;
    Node(Node&&) = delete;
    Node& operator=(const Node& other) = delete;
    Node& operator=(Node&& other) = delete;

    // Duplicates TrieNode
    std::unique_ptr<Node> clone();

    // Recursive function to insert a string in a node
    // parent might be null if this is the root node
    // if parent is not null then parent_child_idx is the index from children_ptr
    // that points to this node (if parent is null the value means nothing)
    uint64_t insert_string(Node* parent, size_t parent_child_idx, const uint8_t* str);

    // Returns the pointer to this node (combination of page_number and page_offset)
    uint64_t node_pos()
    {
        return page->get_page_number() * Page::SIZE + page_offset;
    }

    // Print tire in dot format
    void print_trie_node(std::ostream& os);

private:
    Trie& trie;

    Page* page;
    uint64_t page_offset;

    // The following are pointers to all the node contents
    uint8_t* node_id_ptr; // 5B
    uint8_t* capacity_ptr; // 2B
    uint8_t* document_count_ptr; // 1B
    uint8_t* str_len_ptr; // 1B
    uint8_t* child_count_ptr; // 1B
    uint8_t* string_ptr; // (1B char) * string_length
    uint8_t* children_ptr; // (1B char, 5B Page Pointer) * child_count

    // parent might be null if this is the root node
    // if parent is not null then parent_child points inside the array represented by children_ptr
    // effectively is who points to this node
    // returns the new leaf (might not be the children but deeper a descendant)
    std::unique_ptr<Node> insert_child(Node* parent, size_t parent_child_idx, const uint8_t* string);

    // Split this node, inserting a new node between parent and this node.
    // parent might be null if this is the root node
    // if parent is not null then parent_child points inside the array represented by children_ptr
    // effectively is who points to this node
    // split_position indicates where to split.
    // For example:
    // - split_position=0: "abc" -> "", "abc"
    // - split_position=1: "abc" -> "a", "bc"
    // Returns the newly created node.
    std::unique_ptr<Node> split(Node* parent, size_t parent_child_idx, size_t split_position);

    void update_child_page_pointer(size_t parent_child_idx, uint64_t node_pos);

    // Calculates space needed for a node with string_length and child_count
    size_t size(size_t string_length, size_t child_count) const;

    // if page is in read-only mode, replace with an editable page
    void upgrade_to_editable();

    // Increments the document count at the current node.
    // Mainly used for debugging, and potentially be removed.
    // Instead we could rely on whether the node_id of the trie
    // node is present in the BPT.
    void increment_document_count();

    // Initialize pointers of an existing Node using the counts in the Page
    void init_pointers_using_page();

    // Initialize pointers of a new Node using the counts passed as arguments
    void init_pointers_using_string_length(size_t string_length);
};

} // namespace TextSearch
