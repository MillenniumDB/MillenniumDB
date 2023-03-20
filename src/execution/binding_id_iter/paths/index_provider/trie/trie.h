#pragma once

#include <vector>

#include "base/ids/object_id.h"

/*
This data structure assumes that the inserted elements are ordered lexicographically.
If the same pair is inserted multiple times, it's stored with duplicates, since the edge IDs connecting them are different.
In a property graph, two nodes can be connected via different edges with the same type.
Ex: Edge(n1,p1,n2,e1), Edge(n1,p1,n2,e2)
*/
// Trie index
struct Trie {
    // Tree levels
    std::vector<uint64_t> prefix_lvl;
    std::vector<uint64_t> data_lvl;

    // Offset indexing
    std::vector<uint32_t> offsets;
    uint32_t data_count = 0;
    uint32_t last_offset = 0;

    // Last prefix inserted in the first level during construction
    uint64_t invalid_object_id = ObjectId::OBJECT_ID_NOT_FOUND;
    uint64_t last_prefix = invalid_object_id;

    // Insert an element into the tree (construction)
    void insert(uint64_t prefix, uint64_t data);

    // Prepare tree for range queries (after construction)
    void end_inserts();

    // Range query using a prefix
    std::pair<uint64_t*, uint64_t*> query(uint64_t prefix);

    // Visualize the entire tree
    void print();

    // Show offsets in the data level
    void print_offsets();
};
