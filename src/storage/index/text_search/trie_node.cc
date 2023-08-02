#include "trie_node.h"

#include <cassert>
#include <cmath>
#include <iostream>

#include "storage/index/text_search/trie.h"
#include "storage/buffer_manager.h"
#include "storage/file_manager.h"
#include "storage/page.h"

TrieNode::TrieNode(Trie& trie, Page& node_page, uint64_t node_pos_in_page) :
    trie (trie),
    node_page (node_page)
{
    // Get bytes from page
    unsigned char* page_begin = reinterpret_cast<unsigned char *> (node_page.get_bytes());

    // Target every attribute (N L V C I)
    child_count = page_begin + node_pos_in_page;
    str_length = child_count + NP_SN;
    leaf_count = str_length + NP_SL;
    capacity   = leaf_count + NP_SV;
    information = capacity + NP_SC;
}


TrieNode::~TrieNode() {
    buffer_manager.unpin(node_page);

    if (info_page != nullptr) {
        buffer_manager.unpin(*info_page);
    }
}


std::unique_ptr<TrieNode> TrieNode::clone() {
    buffer_manager.pin(node_page);
    auto node_pos_in_page = reinterpret_cast<char*>(child_count) - node_page.get_bytes();
    return std::make_unique<TrieNode>(trie, node_page, node_pos_in_page);
}


void TrieNode::split_node(int pos, unsigned char* cpy_str_node) {
    // Store a pointer to where the split will be made
    unsigned char* str_node = cpy_str_node + pos;

    // First, copy the node: (it will be the child of the original node)
    uint64_t searched_node = search_free_node();
    uint64_t copy_offset = searched_node % trie.PAGE_SIZE;
    uint64_t copy_page_num = searched_node / trie.PAGE_SIZE;
    auto& copy_page = buffer_manager.get_page(trie.node_file_id, copy_page_num);
    memcpy(reinterpret_cast<unsigned char *> (copy_page.get_bytes()) + copy_offset, child_count, trie.NODE_SIZE);
    // Note that child_count is the beginning of the node that we need to copy

    // Update the values of the original node
    *child_count = 1; // Only has one child at the moment
    *str_length = pos; // The word will be shortened
    write_xbytes(0, leaf_count, NP_SV); // Has no leaves

    // Check capacity and write
    uint64_t C = trie.CAPACITY;
    while ((uint64_t) (IP_SCh + IP_SO) + pos + IP_SV > C) { C *= 2; }
    // Notice that this node has one child (6B) and one string of length pos (pos B).
    // The last +8 B is because two things can happen to this node: 1) add another child node (6B) or add a leaf (8B).
    // In the worst case, 8B will be added, so we consider it now to avoid another duplication of capacity.

    uint64_t searched_capacity = search_free_capacity(C);
    // Write capacity
    write_xbytes(C, capacity, NP_SC);
    // Update I to the new location:
    write_xbytes(searched_capacity, information, NP_SI);

    // Search info_page and pos_info
    uint64_t info_value = read_xbytes(information, NP_SI);
    Page* info_page = nullptr;
    unsigned char* pos_info = get_info_page_and_pos_info(&info_page, info_value);

    // Update the information of the original node
    *pos_info = *str_node;                                  // character
    write_xbytes(searched_node, pos_info + IP_SCh, IP_SO); // position child node
    for (int i = 0; i < pos; i++) {                         // string of node
        *(pos_info + IP_SCh + IP_SO + i) = *(cpy_str_node + i);
    }

    info_page->make_dirty();
    buffer_manager.unpin(*info_page);

    // The copied node also needs to be updated (L value and string)
    // Gather its data:
    unsigned char* cpy_node_pos = reinterpret_cast<unsigned char *> (copy_page.get_bytes()) + copy_offset;
    int N_cpy_node = *cpy_node_pos;
    int L_cpy_node = *(cpy_node_pos + NP_SN);
    uint64_t V_cpy_node = read_xbytes(cpy_node_pos + NP_SN + NP_SL, NP_SV);
    // C_cpy_node
    uint64_t info_value_cpy_node = read_xbytes(cpy_node_pos + NP_SN + NP_SL + NP_SV + NP_SC, NP_SI);
    Page* info_page_cpy_node = nullptr;
    unsigned char* pos_info_cpy_node = get_info_page_and_pos_info(&info_page_cpy_node, info_value_cpy_node);

    // Update of L (L=5 and pos=1 => new_L=3)
    *(cpy_node_pos + NP_SN) = L_cpy_node - pos - 1;
    L_cpy_node = *(cpy_node_pos + NP_SN);

    // Shift (<-) the information to cut the string ("hello" pos=1 => "llo")
    memmove(pos_info_cpy_node + N_cpy_node*(IP_SCh+IP_SO),
            pos_info_cpy_node + N_cpy_node*(IP_SCh+IP_SO) + pos + 1,
            L_cpy_node + V_cpy_node*IP_SV);

    info_page_cpy_node->make_dirty();
    buffer_manager.unpin(*info_page_cpy_node);

    node_page.make_dirty();
    copy_page.make_dirty();

    buffer_manager.unpin(copy_page);
}


int TrieNode::check_capacity(uint64_t bytes) {
    // Gather the data of the node:
    int N = *child_count;
    int L = *str_length;
    uint64_t V = read_xbytes(leaf_count, NP_SV);
    uint64_t C = read_xbytes(capacity, NP_SC);

    // Calculate the space used:
    int used_space = N*(IP_SCh+IP_SO) + L + V*IP_SV;

    // Check if any changes are required:
    if (used_space + bytes <= C) { return 0; }

    // Obtain the position of the original information
    uint64_t info_value_original = read_xbytes(information, NP_SI);
    Page* info_page_original = nullptr;
    unsigned char* pos_info_original = get_info_page_and_pos_info(&info_page_original, info_value_original);

    if (C != 0) { trie.garbage.add_capacity(C, info_value_original); }

    // Increase the node's capacity as needed
    if (C == 0) { C = trie.CAPACITY; } // If the capacity of the node is 0, set the default capacity
    while (used_space + bytes > C) { C *= 2; }

    uint64_t searched_capacity = search_free_capacity(C);
    // Update the capacity value
    write_xbytes(C, capacity, NP_SC);
    // Update I to the new location:
    write_xbytes(searched_capacity, information, NP_SI);

    // Obtain the position of the new information
    Page* info_page_new = nullptr;
    unsigned char* pos_info_new = get_info_page_and_pos_info(&info_page_new, searched_capacity);

    // Move information to the new location and erase previous data
    memcpy(pos_info_new, pos_info_original, used_space);
    memset(pos_info_original, 0, used_space);

    info_page_original->make_dirty();
    info_page_new->make_dirty();
    buffer_manager.unpin(*info_page_original);
    buffer_manager.unpin(*info_page_new);

    return 1;
}


void TrieNode::check_space_for_node() {
    uint64_t res = trie.PAGE_SIZE - *(trie.last_offset_node_page) % trie.PAGE_SIZE;
    if (res < trie.NODE_SIZE) {
        *(trie.last_offset_node_page) += res;
        trie.root.node_page.make_dirty();
    }
}


void TrieNode::check_space_for_capacity(uint64_t C) {
    uint64_t res = trie.PAGE_SIZE - *(trie.last_offset_info_page) % trie.PAGE_SIZE;
    if (res < C) {
        // Add to garbage if the space is valuable
        if (res >= trie.CAPACITY) {
            trie.garbage.add_capacity(res, *(trie.last_offset_info_page));
        }

        *(trie.last_offset_info_page) += res;
        trie.root.node_page.make_dirty();
    }
}


unsigned char* TrieNode::get_info_page_and_pos_info(Page** info_page, uint64_t pos_info_value) {
    // Unpins if needed
    if (*info_page != nullptr) {
        buffer_manager.unpin(**info_page);
    }

    // Calculate the page number and offset
    uint64_t info_page_num = pos_info_value / trie.PAGE_SIZE;
    uint64_t info_offset = pos_info_value % trie.PAGE_SIZE;

    // Get corresponding info_page
    *info_page = &buffer_manager.get_page(trie.info_file_id, info_page_num);

    // Returns pos_info
    return reinterpret_cast<unsigned char *> ((*info_page)->get_bytes()) + info_offset;
}


void TrieNode::insert(const unsigned char* str, uint64_t id) {
    // Gather the data of the node:
    int N = *child_count;
    int L = *str_length;
    uint64_t V = read_xbytes(leaf_count, NP_SV);
    uint64_t info_value = read_xbytes(information, NP_SI);
    unsigned char* pos_info = get_info_page_and_pos_info(&info_page, info_value);

    // First, check if the received string is empty => add leaf to node
    if (!(*str)) {
        // Check if the node needs to be split
        if (L > 0) {
            // Split in pos=0:
            int pos = 0;
            unsigned char* cpy_str_node = pos_info + N*(IP_SCh+IP_SO);
            split_node(pos, cpy_str_node);

            // Update variables of data
            N = *child_count;
            L = *str_length;
            V = read_xbytes(leaf_count, NP_SV);
            info_value = read_xbytes(information, NP_SI);
            pos_info = get_info_page_and_pos_info(&info_page, info_value);
        }

        // Add leaf:
        // But first, check if the capacity is enough for +8B
        if (check_capacity(IP_SV)) {
            // if the capacity has changed, update information variables
            info_value = read_xbytes(information, NP_SI);
            pos_info = get_info_page_and_pos_info(&info_page, info_value);
        }
        unsigned char* pos_leaf = pos_info + N*(IP_SCh+IP_SO) + L + V*IP_SV;
        write_xbytes(V + 1, leaf_count, NP_SV); // +1 Leaf
        write_xbytes(id, pos_leaf, IP_SV);      // Add id to info_page

        node_page.make_dirty();
        info_page->make_dirty();

        return;
    }

    // Second, compare the string of the node with the received string
    if (L > 0) {
        unsigned char* str_node = pos_info + N*(IP_SCh+IP_SO);
        unsigned char* cpy_str_node = str_node;
        int pos = 0;

        // Letter-by-letter comparison
        while (*str == *str_node && pos < L) {
            str++;
            str_node++;
            pos++;
        }

        // If str_node remains, split the node
        if (pos < L) {
            split_node(pos, cpy_str_node);

            // Update variables of data
            N = *child_count;
            L = *str_length;
            V = read_xbytes(leaf_count, NP_SV);
            info_value = read_xbytes(information, NP_SI);
            pos_info = get_info_page_and_pos_info(&info_page, info_value);
        }

        // If the received string is finished, add leaf
        if (!(*str)) {
            // Add leaf:
            // But first, check if the capacity is enough for +8B
            if (check_capacity(IP_SV)) {
                // if the capacity has changed, update information variables
                info_value = read_xbytes(information, NP_SI);
                pos_info = get_info_page_and_pos_info(&info_page, info_value);
            }
            unsigned char* pos_leaf = pos_info + N*(IP_SCh+IP_SO) + L + V*IP_SV;
            write_xbytes(V + 1, leaf_count, NP_SV); // +1 leaf
            write_xbytes(id, pos_leaf, IP_SV);      // Add id to info_page

            node_page.make_dirty();
            info_page->make_dirty();

            return;
        }
    }

    // Search next child
    int found = 0;
    int i = 0;
    for (i = 0; i < N; i++) {
        // std::cout << *pos_info << std::endl;
        if (*str == *pos_info) { found = 1; break; }
        if (*str < *pos_info) { break; } // To maintain the children's order
        pos_info += IP_SCh + IP_SO;
    }

    // If it does not exist, we must create it
    if (found == 0) {
        // Add child:
        // But first, check if the capacity is enough for +6B
        if (check_capacity(IP_SCh + IP_SO)) {
            // if the capacity has changed, update information variables
            info_value = read_xbytes(information, NP_SI);
            pos_info = get_info_page_and_pos_info(&info_page, info_value) + i*(IP_SCh + IP_SO);
        }

        // Update father information
        *child_count += 1;                                              // N
        memmove(pos_info + IP_SCh + IP_SO, pos_info,
                (N-i)*(IP_SCh+IP_SO) + L + V*IP_SV);                   // Shift (-> 6 B) of the remaining children,
                                                                        // string of node and leaves.
        *pos_info = *str;                                               // Add character of child.
        uint64_t searched_node = search_free_node();
        write_xbytes(searched_node, pos_info + IP_SCh, IP_SO);         // Add offset of new child.

        node_page.make_dirty();
        info_page->make_dirty();

        // Update child information
        // Get position of child:
        uint64_t child_offset = searched_node % trie.PAGE_SIZE;
        uint64_t child_page_num = searched_node / trie.PAGE_SIZE;
        Page* child_page = &buffer_manager.get_page(trie.node_file_id, child_page_num);

        // Get the size of the string to be added
        int size = strlen(reinterpret_cast<const char *> (++str));

        // If the word is longer than 255 characters, we will cut it and pass the rest to the next node
        while (size > MAX_STR_LENGTH_IN_NODE) {
            unsigned char * child_pos = reinterpret_cast<unsigned char *> (child_page->get_bytes()) + child_offset;
            // In this case, the child will not have a leaf but another child
            // Write the corresponding data:
            *(child_pos) = 1;                                 // N = 1
            *(child_pos + NP_SN) = MAX_STR_LENGTH_IN_NODE;    // L = 255
            // Has no leaves

            // Calculate the capacity and write
            uint64_t C = trie.CAPACITY;
            while ((IP_SCh+IP_SO) + MAX_STR_LENGTH_IN_NODE > C) { C *= 2; } // One child (6B) and 255 characters (255B)

            uint64_t searched_capacity = search_free_capacity(C);
            // Write capacity
            write_xbytes(C, child_pos + NP_SN + NP_SL + NP_SV, NP_SC);
            // Write information (I) according to the capacity
            write_xbytes(searched_capacity, child_pos + NP_SN + NP_SL + NP_SV + NP_SC, NP_SI);

            // Get info_page
            Page* info_page_child = nullptr;
            unsigned char* pos_info_child = get_info_page_and_pos_info(&info_page_child, searched_capacity);

            // Write string
            for (int i = 0; i < MAX_STR_LENGTH_IN_NODE; i++) {
                *(pos_info_child + (IP_SCh+IP_SO) + i) = *(str++);
            }

            // Write new child to this child (character and offset)
            *(pos_info_child) = *str; // Character
            searched_node = search_free_node();
            write_xbytes(searched_node, pos_info_child + IP_SCh, IP_SO); // Offset

            // Unpins and dirty
            child_page->make_dirty();
            buffer_manager.unpin(*child_page);
            info_page_child->make_dirty();
            buffer_manager.unpin(*info_page_child);

            // Get position of next child:
            child_offset = searched_node % trie.PAGE_SIZE;
            child_page_num = searched_node / trie.PAGE_SIZE;
            child_page = &buffer_manager.get_page(trie.node_file_id, child_page_num);

            // Update size variable (while)
            size = strlen(reinterpret_cast<const char *> (++str));
        }

        unsigned char * child_pos = reinterpret_cast<unsigned char *> (child_page->get_bytes()) + child_offset;

        // This child will have only a leaf and a string
        // Write the corresponding data:
        *(child_pos + NP_SN) = size;                        // L
        write_xbytes(1, child_pos + NP_SN + NP_SL, NP_SV);  // V

        // Calculate the capacity and write
        uint64_t C = trie.CAPACITY;
        while ((uint64_t)size + IP_SV > C) { C *= 2; } // The string (size B) and one leaf (8B)

        uint64_t searched_capacity = search_free_capacity(C);
        // Write capacity
        write_xbytes(C, child_pos + NP_SN + NP_SL + NP_SV, NP_SC);
        // Write information (I) according to the capacity
        write_xbytes(searched_capacity, child_pos + NP_SN + NP_SL + NP_SV + NP_SC, NP_SI);

        // Get info_page
        Page* info_page_child = nullptr;
        unsigned char* pos_info_child = get_info_page_and_pos_info(&info_page_child, searched_capacity);

        // Write string and id of word
        for (int i = 0; i < size; i++) {
            *(pos_info_child + i) = *(str + i);
        }
        write_xbytes(id, pos_info_child + size, IP_SV);

        // Unpins and dirty
        child_page->make_dirty();
        buffer_manager.unpin(*child_page);
        info_page_child->make_dirty();
        buffer_manager.unpin(*info_page_child);

        return;
    }

    // Recursive call of the following node
    uint64_t next_node_pos = read_xbytes(pos_info + IP_SCh, IP_SO);
    uint64_t next_node_offset = next_node_pos % trie.PAGE_SIZE;
    uint64_t next_node_page_num = next_node_pos / trie.PAGE_SIZE;
    auto& next_node_page = buffer_manager.get_page(trie.node_file_id, next_node_page_num);
    TrieNode next_node(trie, next_node_page, next_node_offset);

    next_node.insert(++str, id);
}


TrieIter TrieNode::get_iter(const unsigned char* str, bool exact_search) {
    // Gather the data of the node:
    int N = *child_count;
    int L = *str_length;
    // uint64_t V = read_xbytes(leaf_count, NP_SV);
    uint64_t info_value = read_xbytes(information, NP_SI);
    unsigned char* pos_info = get_info_page_and_pos_info(&info_page, info_value);

    // If the string is finished, we found a match with the prefix or exact
    if (!(*str)) {
        // Exact Search:
        if (exact_search) {
            if (L == 0) { return TrieIter(clone(), true); }
            else { return TrieIter(); }
        }

        // Prefix Search:
        return TrieIter(clone());
    }

    // If the node has an associated string
    if (L > 0) {
        unsigned char* str_node = pos_info + N*(IP_SCh+IP_SO);
        int pos = 0;
        while (*str == *str_node && pos < L) {
            str++;
            str_node++;
            pos++;
        }

        // If the string is finished, we found a match with the prefix
        if (!(*str)) {
            // Exact Search:
            if (exact_search) {
                if (pos == L) { return TrieIter(clone(), true); }
                else { return TrieIter(); }
            }

            // Prefix Search:
            return TrieIter(clone());
        }
        if (pos < L) {
            return TrieIter();
        }
    }

    // Search next child
    int found = 0;
    for (int i = 0; i < N; i++) {
        if (*str == *pos_info) { found = 1; break; }
        pos_info += IP_SCh + IP_SO;
    }

    // If found, recursive call of the following node
    if (found == 1) {
        uint64_t next_node_pos = read_xbytes(pos_info + IP_SCh, IP_SO);
        uint64_t next_node_offset = next_node_pos % trie.PAGE_SIZE;
        uint64_t next_node_page_num = next_node_pos / trie.PAGE_SIZE;
        auto& next_node_page = buffer_manager.get_page(trie.node_file_id, next_node_page_num);
        TrieNode next_node(trie, next_node_page, next_node_offset);

        return next_node.get_iter(++str, exact_search);
    }

    return TrieIter();
}


void TrieNode::print_trie_node() {
    // Gather the data of the node:
    int N = *child_count;
    int L = *str_length;
    uint64_t V = read_xbytes(leaf_count, NP_SV);
    uint64_t info_value = read_xbytes(information, NP_SI);
    unsigned char* pos_info = get_info_page_and_pos_info(&info_page, info_value);

    std::cout << ":";
    std::cout << "(";

    // Print string of node
    for (int i = 0; i < L; i++) {
        std::cout << *(pos_info + N*(IP_SCh+IP_SO) + i);
    }

    std::cout << ")[";

    // Print leaves
    for (uint64_t i = 0; i < V; i++) {
        std::cout << read_xbytes(pos_info + N*(IP_SCh+IP_SO) + L + i*IP_SV, IP_SV) << ",";
    }
    std::cout << "]{";

    // Recursive call of children
    char letter;
    uint64_t next_node_pos;
    uint64_t next_node_offset;
    uint64_t next_node_page_num;
    for (int i = 0; i < N; i++) {
        letter = *(pos_info + i*(IP_SCh+IP_SO));
        std::cout << letter;

        next_node_pos = read_xbytes(pos_info + IP_SCh + i*(IP_SCh+IP_SO), IP_SO);
        next_node_offset = next_node_pos % trie.PAGE_SIZE;
        next_node_page_num = next_node_pos / trie.PAGE_SIZE;
        auto& next_node_page = buffer_manager.get_page(trie.node_file_id, next_node_page_num);
        TrieNode next_node(trie, next_node_page, next_node_offset);
        next_node.print_trie_node();

        std::cout << ",";
    }
    std::cout << "}";
}


int TrieNode::deletion(const unsigned char* str) {
    // Gather the data of the node:
    int N = *child_count;
    int L = *str_length;
    uint64_t V = read_xbytes(leaf_count, NP_SV);
    uint64_t C = read_xbytes(capacity, NP_SC);
    uint64_t info_value = read_xbytes(information, NP_SI);
    unsigned char* pos_info = get_info_page_and_pos_info(&info_page, info_value);
    unsigned char* begin_info = pos_info;

    // If the node has an associated string
    if (L > 0) {
        // Letter-by-letter comparison
        unsigned char* str_node = pos_info + N*(IP_SCh+IP_SO);
        int pos = 0;
        while (*str == *str_node && pos < L) {
            str++;
            str_node++;
            pos++;
        }

        // If both the node's string and the entered string are finished, we must eliminate
        if (!(*str) && pos == L) {
            // Erase the node if it has no children
            if (N == 0) {
                // Add to garbage
                uint64_t node_pos = node_page.page_id.page_number * trie.PAGE_SIZE +
                                    (child_count - reinterpret_cast<unsigned char *> (node_page.get_bytes()));
                if (node_pos != trie.root_node_offset) { trie.garbage.add_node(node_pos); }
                trie.garbage.add_capacity(C, info_value);

                // Erase
                memset(child_count, 0, trie.NODE_SIZE);
                memset(begin_info, 0, C);
                node_page.make_dirty();
                info_page->make_dirty();
                return 1;
            }

            // Otherwise, erase the leaves
            else {
                memset(begin_info + N*(IP_SCh+IP_SO) + L, 0, V*IP_SV);
                write_xbytes(0, leaf_count, NP_SV);
                node_page.make_dirty();
                info_page->make_dirty();
                return 0;
            }
        }

        // if the entered string is finished but not the node's string, nothing is deleted.
        if (!(*str) && pos < L) {
            return 0;
        }
    }

    // If the entered string is still not finished, we try to go to the next node
    if (*str) {
        // Search next child
        int found = 0;
        int i = 0;
        for (i = 0; i < N; i++) {
            // std::cout << *pos_info << std::endl;
            if (*str == *pos_info) { found = 1; break; }
            pos_info += IP_SCh + IP_SO;
        }

        // If not found, nothing is done
        if (found == 0) { return 0; }

        // Otherwise, the next child is called
        uint64_t next_node_pos = read_xbytes(pos_info + IP_SCh, IP_SO);
        uint64_t next_node_offset = next_node_pos % trie.PAGE_SIZE;
        uint64_t next_node_page_num = next_node_pos / trie.PAGE_SIZE;
        auto& next_node_page = buffer_manager.get_page(trie.node_file_id, next_node_page_num);
        TrieNode next_node(trie, next_node_page, next_node_offset);

        if (next_node.deletion(++str)) {
            // If a child is deleted, we update the number of children and remove its entry
            *child_count -= 1;
            N = *child_count;
            memset(pos_info, 0, IP_SCh + IP_SO);
            // Also shift (<-) the rest of the information
            memmove(pos_info, pos_info + IP_SCh + IP_SO, (N-i)*(IP_SCh+IP_SO) + L + V*IP_SV);

            node_page.make_dirty();
            info_page->make_dirty();

            // If there are no children left and this node has no leaves, the node is deleted.
            if (N == 0 && V == 0) {
                // Add to garbage
                uint64_t node_pos = node_page.page_id.page_number * trie.PAGE_SIZE +
                                    (child_count - reinterpret_cast<unsigned char *> (node_page.get_bytes()));
                if (node_pos != trie.root_node_offset) { trie.garbage.add_node(node_pos); }
                trie.garbage.add_capacity(C, info_value);

                // Erase
                memset(child_count, 0, trie.NODE_SIZE); // Erase the node
                memset(begin_info, 0, C);               // Erase the node information
                return 1;
            }

            // Otherwise, do not eliminate
            else { return 0; }
        }
    }

    // If the entered string is finished, node has no string and has leaves, delete
    if (!(*str) && V > 0 && L == 0) {
        // Erase the node if it has no children
        if (N == 0) {
            // Add to garbage
            uint64_t node_pos = node_page.page_id.page_number * trie.PAGE_SIZE +
                                (child_count - reinterpret_cast<unsigned char *> (node_page.get_bytes()));
            if (node_pos != trie.root_node_offset) { trie.garbage.add_node(node_pos); }
            trie.garbage.add_capacity(C, info_value);

            // Erase
            memset(child_count, 0, trie.NODE_SIZE);
            memset(begin_info, 0, C);
            node_page.make_dirty();
            info_page->make_dirty();
            return 1;
        }

        // Otherwise, erase the leaves
        else {
            memset(begin_info + N*(IP_SCh+IP_SO) + L, 0, V*IP_SV);
            write_xbytes(0, leaf_count, NP_SV);
            node_page.make_dirty();
            info_page->make_dirty();
            return 0;
        }
    }

    // In all other cases, nothing is done
    return 0;
}

uint64_t TrieNode::search_free_capacity(uint64_t& C) {
    // Search available free space in garbage (For capacity)
    uint64_t value;
    if (trie.garbage.search_and_pop_capacity(C, value)) { return value; }

    // If nothing is found in garbage, we need to check if the capacity fits in the remainder of the page
    check_space_for_capacity(C);
    // and get last offset:
    value = *trie.last_offset_info_page;

    // Update offset for the next node that needs it
    *(trie.last_offset_info_page) += C;
    trie.root.node_page.make_dirty();

    return value;
}

uint64_t TrieNode::search_free_node() {
    // Search available free space in garbage (For node)
    uint64_t value;
    if (trie.garbage.search_and_pop_node(value)) { return value; }

    // If nothing is found in garbage, get last offset:
    value = *trie.last_offset_node_page;

    // Update offset for the next new node
    *(trie.last_offset_node_page) += trie.NODE_SIZE;
    trie.root.node_page.make_dirty();

    // Check if there is enough free space in page for the next new node
    check_space_for_node();

    return value;
}