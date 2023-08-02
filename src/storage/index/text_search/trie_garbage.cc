#include "trie_garbage.h"

#include "storage/buffer_manager.h"
#include "storage/file_manager.h"
#include "storage/page.h"

#include <cmath>
#include <iostream>
#include <cassert>


TrieGarbage::TrieGarbage() :
    garbage_file_id (file_manager.get_file_id("text_search.garbage")),
    dir_page (buffer_manager.get_page(garbage_file_id, 0))
{
    // Get bytes from page
    unsigned char* dir_page_begin = reinterpret_cast<unsigned char *> (dir_page.get_bytes());

    // Target every attribute
    capacities_count = dir_page_begin;
    last_page = dir_page_begin + DP_SN;
    first_and_last_pages = last_page + DP_SLP;
}

TrieGarbage::~TrieGarbage() {
    buffer_manager.unpin(dir_page);
}


void TrieGarbage::add_capacity(uint64_t capacity, uint64_t value) {
    // Gather directory data
    int N = *capacities_count;
    uint64_t LP = read_xbytes(last_page, DP_SLP);

    // Obtain position of capacity
    int pos = log2(capacity) - OFFSET_SLOT_CAPACITY; // 16B -> 1, 32B -> 2, 64B -> 3, ...

    assert(pos >= 0);

    // Update N if pos is new
    if (N < pos + 1) {
        *capacities_count = (unsigned char) pos + 1;
        dir_page.make_dirty();
    }

    // Get first and last page of the associated capacity
    // (For every category of garbage: 4 bytes first page and 4 bytes last page)
    uint64_t first_page_of_capacity = read_xbytes(first_and_last_pages + pos*(DP_FLPs1+DP_FLPs2), DP_FLPs1);
    uint64_t last_page_of_capacity = read_xbytes(first_and_last_pages + pos*(DP_FLPs1+DP_FLPs2) + DP_FLPs1, DP_FLPs2);
    uint64_t target_page = last_page_of_capacity;

    // If first_page is 0, we need to create a page
    if (first_page_of_capacity == 0) {
        // Update directory:
        // - General last page
        write_xbytes(++LP, last_page, DP_SLP);
        // - First page of this capacity
        write_xbytes(LP, first_and_last_pages + pos*(DP_FLPs1+DP_FLPs2), DP_FLPs1);
        // - Last page of this capacity
        write_xbytes(LP, first_and_last_pages + pos*(DP_FLPs1+DP_FLPs2) + DP_FLPs1, DP_FLPs2);

        target_page = LP;

        dir_page.make_dirty();
    }

    // If last_page is 0 and first_page != 0, there are no values, so we need to insert in the first page
    else if (last_page_of_capacity == 0) {
        // Update last page of this capacity
        write_xbytes(first_page_of_capacity, first_and_last_pages + pos*(DP_FLPs1+DP_FLPs2) + DP_FLPs1, DP_FLPs2);
        target_page = first_page_of_capacity;
        dir_page.make_dirty();
    }

    Page& page = buffer_manager.get_page(garbage_file_id, target_page);

    // Write new value:
    unsigned char* page_pos = reinterpret_cast<unsigned char*> (page.get_bytes());
    uint64_t value_count = read_xbytes(page_pos, OP_SN);
    // If there is no space in page, go to next page
    if (value_count == MAX_VALUES_IN_PAGE) {
        // Get next page number
        // (2B Number of values, 4B Previous page, 4B Next page)
        uint64_t next_page_num = read_xbytes(page_pos + OP_SN + OP_SPP, OP_SNP);

        // If next_page_num is 0 we need to append a new page
        if (next_page_num == 0) {
            next_page_num = ++LP;
            write_xbytes(next_page_num, page_pos + OP_SN + OP_SPP, OP_SNP);
        }

        // Edit next page information and add value
        Page& next_page = buffer_manager.get_page(garbage_file_id, next_page_num);
        unsigned char* next_page_pos = reinterpret_cast<unsigned char *> (next_page.get_bytes());
        write_xbytes(1, next_page_pos, OP_SN); // Value count = 1
        write_xbytes(target_page, next_page_pos + OP_SN, OP_SPP); // Previous page

        // Add value
        write_xbytes(value, next_page_pos + OP_SN + OP_SPP + OP_SNP, OP_SO);

        next_page.make_dirty();
        buffer_manager.unpin(next_page);

        // Update directory
        // - If LP changed:
        write_xbytes(LP, last_page, DP_SLP);
        // - Update last page of this capacity:
        write_xbytes(next_page_num, first_and_last_pages + pos*(DP_FLPs1+DP_FLPs2) + DP_FLPs1, DP_FLPs2);
        dir_page.make_dirty();
    }
    // Add value normally
    else {
        write_xbytes(value_count+1, page_pos, OP_SN);
        // (2B Number of values, 4B Previous page, 4B Next page, 5B for each value)
        write_xbytes(value, page_pos + OP_SN + OP_SPP + OP_SNP + OP_SO*value_count, OP_SO);
    }

    page.make_dirty();
    buffer_manager.unpin(page);
}


int TrieGarbage::search_and_pop_capacity(uint64_t& capacity, uint64_t& value) {
    // Gather directory data
    int N = *capacities_count;
    // uint64_t LP = read_xbytes(last_page, DP_SLP);

    // Obtain position of capacity
    int pos = log2(capacity) - OFFSET_SLOT_CAPACITY; // 16B -> 1, 32B -> 2, ...
    assert(pos >= 0);

    // Search free space (>= capacity)
    for (int i = pos; i < N; i++) {
        // Get the last written page of the capacity associated with i
        // (For every category of garbage: 4 bytes first page and 4 bytes last page)
        uint64_t last_page_of_capacity = read_xbytes(first_and_last_pages + i*(DP_FLPs1+DP_FLPs2) + DP_FLPs1, DP_FLPs2);
        if (last_page_of_capacity == 0) { continue; } // If no values, next capacity
        Page& page = buffer_manager.get_page(garbage_file_id, last_page_of_capacity);
        unsigned char* page_pos = reinterpret_cast<unsigned char *> (page.get_bytes());

        // Get number of values in page
        int value_count = read_xbytes(page_pos, OP_SN);
        assert(value_count > 0);

        // Pop value:

        // Read last value
        // (2B Number of values, 4B Previous page, 4B Next page, 5B for each value)
        value = read_xbytes(page_pos + OP_SN + OP_SPP + OP_SNP + OP_SO*(value_count - 1), OP_SO);

        // Update capacity in case it has increased
        capacity = pow(2, i+OFFSET_SLOT_CAPACITY);

        // Erase last value
        memset(page_pos + OP_SN + OP_SPP + OP_SNP + OP_SO*(value_count - 1), 0, OP_SO);

        // Update count
        write_xbytes(--value_count, page_pos, OP_SN);

        // Check if the page became empty
        if (value_count == 0) {
            // Read previous page of empty page
            uint64_t prev_page = read_xbytes(page_pos + OP_SN, OP_SPP);

            // Update directory (Last page of this capacity)
            write_xbytes(prev_page, first_and_last_pages + i*(DP_FLPs1+DP_FLPs2) + DP_FLPs1, DP_FLPs2);
            dir_page.make_dirty();
        }

        page.make_dirty();
        buffer_manager.unpin(page);

        return 1;
    }

    // Nothing is found
    return 0;
}


void TrieGarbage::add_node(uint64_t value) {
    // Gather directory data
    int N = *capacities_count;
    uint64_t LP = read_xbytes(last_page, DP_SLP);

    // Position of nodes is 0
    int pos = 0;

    // Update N if pos is new
    if (N < pos + 1) {
        *capacities_count = (unsigned char) pos + 1;
        dir_page.make_dirty();
    }

    // Get first and last page of the garbage associated with nodes
    // (4 bytes first page and 4 bytes last page)
    uint64_t first_page_of_node = read_xbytes(first_and_last_pages, DP_FLPs1);
    uint64_t last_page_of_node = read_xbytes(first_and_last_pages + DP_FLPs1, DP_FLPs2);
    uint64_t target_page = last_page_of_node;

    // If first_page is 0, we need to create a page
    if (first_page_of_node == 0) {
        // Update directory
        write_xbytes(++LP, last_page, DP_SLP); // General last page
        write_xbytes(LP, first_and_last_pages, DP_FLPs1); // First page of nodes
        write_xbytes(LP, first_and_last_pages + DP_FLPs1, DP_FLPs2); // Last page of nodes

        target_page = LP;

        dir_page.make_dirty();
    }

    // If last_page is 0 and first_page != 0, there are no values, so we need to insert in the first page
    else if (last_page_of_node == 0) {
        // Update last page of nodes
        write_xbytes(first_page_of_node, first_and_last_pages + DP_FLPs1, DP_FLPs2);
        target_page = first_page_of_node;
        dir_page.make_dirty();
    }

    Page& page = buffer_manager.get_page(garbage_file_id, target_page);

    // Write new value:
    unsigned char* page_pos = reinterpret_cast<unsigned char*> (page.get_bytes());
    uint64_t value_count = read_xbytes(page_pos, OP_SN);
    // If there is no space in page, go to next page
    if (value_count == MAX_VALUES_IN_PAGE) {
        // Get next page number
        // (2B Number of values, 4B Previous page, 4B Next page)
        uint64_t next_page_num = read_xbytes(page_pos + OP_SN + OP_SPP, OP_SNP);

        // If next_page_num is 0 we need to append a new page
        if (next_page_num == 0) {
            next_page_num = ++LP;
            write_xbytes(next_page_num, page_pos + OP_SN + OP_SPP, OP_SNP);
        }

        // Edit next page information and add value
        Page& next_page = buffer_manager.get_page(garbage_file_id, next_page_num);
        unsigned char* next_page_pos = reinterpret_cast<unsigned char *> (next_page.get_bytes());
        write_xbytes(1, next_page_pos, OP_SN); // Value count = 1
        write_xbytes(target_page, next_page_pos + OP_SN, OP_SPP); // Previous page

        // Add value
        write_xbytes(value, next_page_pos + OP_SN + OP_SPP + OP_SNP, OP_SO);

        next_page.make_dirty();
        buffer_manager.unpin(next_page);

        // Update directory
        write_xbytes(LP, last_page, DP_SLP); // If LP changed
        write_xbytes(next_page_num, first_and_last_pages + DP_FLPs1, DP_FLPs2); // Update last page of nodes
        dir_page.make_dirty();
    }
    // Add value normally
    else {
        write_xbytes(value_count+1, page_pos, OP_SN);
        // (2B Number of values, 4B Previous page, 4B Next page, 5B for each value)
        write_xbytes(value, page_pos + OP_SN + OP_SPP + OP_SNP + OP_SO*value_count, OP_SO);
    }

    page.make_dirty();
    buffer_manager.unpin(page);
}


int TrieGarbage::search_and_pop_node(uint64_t& value) {
    // Search free space for node

    // Get the last written page of of the garbage associated with nodes
    // (4 bytes first page and 4 bytes last page)
    uint64_t last_page_of_node = read_xbytes(first_and_last_pages + DP_FLPs1, DP_FLPs2);

    // If there is nothing in garbage, return 0
    if (last_page_of_node == 0) { return 0; }

    // Else get page
    Page& page = buffer_manager.get_page(garbage_file_id, last_page_of_node);
    unsigned char* page_pos = reinterpret_cast<unsigned char *> (page.get_bytes());

    // Get number of values in page
    int value_count = read_xbytes(page_pos, OP_SN);
    assert(value_count > 0);

    // Pop value:

    // Read last value
    // (2B Number of values, 4B Previous page, 4B Next page, 5B for each value)
    value = read_xbytes(page_pos + OP_SN + OP_SPP + OP_SNP + OP_SO*(value_count - 1), OP_SO);

    // Erase last value
    memset(page_pos + OP_SN + OP_SPP + OP_SNP + OP_SO*(value_count - 1), 0, OP_SO);

    // Update count
    write_xbytes(--value_count, page_pos, OP_SN);

    // // Check if the page became empty
    if (value_count == 0) {
        // Read previous page of empty page
        uint64_t prev_page = read_xbytes(page_pos + OP_SN, OP_SPP);

        // Update directory (Last page of nodes)
        write_xbytes(prev_page, first_and_last_pages + DP_FLPs1, DP_FLPs2);
        dir_page.make_dirty();
    }

    page.make_dirty();
    buffer_manager.unpin(page);

    return 1;
}


void TrieGarbage::status() {
    std::cout << "STATUS GARBAGE\n";

    // Gather directory data
    int N = *capacities_count;
    // 0 -> Nodes, 1 -> 16B, 2 -> 32B, ...

    for (int i = 0; i < N; i++) {
        // Get first page of the garbage of this category
        uint64_t curr_page = read_xbytes(first_and_last_pages + i*(DP_FLPs1+DP_FLPs2), DP_FLPs1);

        uint64_t counter = 0;
        while (curr_page != 0) {
            // Get page
            Page& page = buffer_manager.get_page(garbage_file_id, curr_page);
            unsigned char* page_pos = reinterpret_cast<unsigned char*> (page.get_bytes());

            // Update count
            counter += read_xbytes(page_pos, OP_SN);

            // Next page
            curr_page = read_xbytes(page_pos + OP_SN + OP_SPP, OP_SNP);

            buffer_manager.unpin(page);
        }

        // Prints
        if (i == 0) { std::cout << "Nodes: \t\t" << counter << std::endl; }
        else { std::cout << "Capacity " << (int) pow(2, i+OFFSET_SLOT_CAPACITY) << ": \t" << counter << std::endl; }
    }
}