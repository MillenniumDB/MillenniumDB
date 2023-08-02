#pragma once

#include <cstdint>
#include <cstring>
#include <memory>
#include <string>
#include <vector>

#include "storage/index/text_search/trie_iter.h"
#include "storage/file_id.h"
#include "storage/page.h"

class Trie;
class TrieIter;
struct ErrorSearchQueue;

class TrieNode {
friend class TrieIter;
friend class TrieErrorIter;
friend class TrieErrorSubIter;
public:
    Trie& trie;

    // Data of TrieNode is separated in node_page (N L V C I) and info_page (information of node).
    Page& node_page;
    Page* info_page = nullptr;

    // Node_page:
    unsigned char* child_count;        // N (1B) Number of children
    unsigned char* str_length;         // L (1B) Length of string in node
    unsigned char* leaf_count;         // V (2B) Number of leaves
    unsigned char* capacity;           // C (2B) Capacity in information page
    unsigned char* information;        // I (5B) Value to obtain information page number and offset
    // Constants of sizes:
    static constexpr int NP_SN = 1;    // Size of N (Node_page)
    static constexpr int NP_SL = 1;    // Size of L (Node_page)
    static constexpr int NP_SV = 2;    // Size of V (Node_page)
    static constexpr int NP_SC = 2;    // Size of C (Node_page)
    static constexpr int NP_SI = 5;    // Size of I (Node_page)
    static constexpr int TOTAL_NODE_SIZE = NP_SN + NP_SL + NP_SV + NP_SC + NP_SI; // Total: 11B


    // Info_page:
    // Info_page data is structured as follows (in order) for every node:
    // - Character of child (1B) and global offset of child (5B) -> N times
    // - 1B for every character of the string associated with the node (L bytes in total)
    // - Ids of words stored in the corresponding node (8B each one) -> V times
    // Constants of sizes:
    static constexpr int IP_SCh = 1;   // Size of character of child (Info_page)
    static constexpr int IP_SO  = 5;   // Size of global offset of child (Info_page)
    static constexpr int IP_SL  = 1;   // Size of each character of the string associated with the node (Info_page)
    static constexpr int IP_SV  = 8;   // Size of each id stored in the node (Info_page)
    // All the information of one node in info_page is stored in a block of capacity C bytes.
    // If the capacity is exceeded, a larger block is assigned.

    // Remark: IP_SCh and IP_SL are considered as 1 in the code, so changing those values requires a major modification (read_xbytes and write_xbytes).
    // For example, comparisons between strings are byte by byte.

    // Other constants:
    static constexpr int MAX_STR_LENGTH_IN_NODE = 255;

    // Constructor of TrieNode
    TrieNode(Trie& trie, Page& node_page, uint64_t node_pos_in_page);

    // Destructor of TrieNode
    ~TrieNode();

    // Duplicates TrieNode
    std::unique_ptr<TrieNode> clone();

    // Recursive function to insert a string in a node
    void insert(const unsigned char* str, uint64_t id);

    // Recursive function that searches for the node corresponding to a prefix.
    // Returns TrieIter.
    // Also allows exact search of word (not prefix) with the boolean exact_search.
    TrieIter get_iter(const unsigned char* str, bool exact_search);

    // Recursive function (debug) that prints the Trie from a specific node.
    void print_trie_node();

    // Recursive function that deletes a word from the Trie
    int deletion(const unsigned char* str);

private:
    // Function to split a node
    // - pos: int indicating the location of the cut to be made (Ex: pos=1 y "hello" => split in letter "e")
    // - cpy_str_node: pointer to the string of the node in info_page
    void split_node(int pos, unsigned char* cpy_str_node);

    // Function that checks if there is enough free space in capacity to write certain number of bytes.
    // If not, doubles the capacity as many times as needed.
    // Returns 1 if the capacity was increased and 0 otherwise.
    int check_capacity(uint64_t bytes);

    // Function that checks if a node fits in the remainder of the page.
    // If not, increases the offset to get to the next node_page.
    void check_space_for_node();

    // Function that checks if the intended capacity (C) fits in the remainder of the page.
    // If not, increases the offset to get to the next info_page.
    void check_space_for_capacity(uint64_t C);

    // Function to obtain the associated info_page (reassign) and returns the position of the information in
    // unsigned char*. Also unpins the information page if != nullptr.
    // Note that pos_info_value is the value of I.
    unsigned char* get_info_page_and_pos_info(Page** info_page, uint64_t pos_info_value);

    // Function to search a free space with capacity C or more in garbage
    uint64_t search_free_capacity(uint64_t& C);

    // Function to search a free space for a node in garbage
    uint64_t search_free_node();

    // Function to write value using B bytes from pos
    static inline void write_xbytes(uint64_t value, unsigned char* pos, int B) {
        for (int i = 0, shift = 0; i < B; ++i, shift += 8) {
            pos[i] = (value >> shift) & 0xFF;
        }
    }

    // Function to read B bytes from pos
    static inline uint64_t read_xbytes(unsigned char* pos, int B) {
        uint64_t value = 0;

        for (int i = 0, shift = 0; i < B; ++i, shift += 8) {
            value |= static_cast<uint64_t>(pos[i]) << shift;
        }

        return value;
    }
};
