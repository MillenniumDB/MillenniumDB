#include <iostream>

#include "edge_trie.h"

using namespace std;


void EdgeTrie::insert(uint64_t prefix, uint64_t node_data, uint64_t edge_data) {
    // Check if prefix is already in the tree
    bool new_prefix = prefix != last_prefix;

    // Add new prefix
    if (new_prefix) {
        // New prefix being inserted
        last_prefix = prefix;

        // Store prefix
        prefix_lvl.push_back(prefix);

        // Store last offset
        offsets.push_back(last_offset + data_count);
        last_offset += data_count;
        data_count = 0;
    }

    // Store data
    data_lvl.push_back(make_pair(node_data, edge_data));
    data_count++;
}


void EdgeTrie::end_inserts() {
    // Store last offset
    offsets.push_back(last_offset + data_count);
}


pair<pair<uint64_t, uint64_t>*, pair<uint64_t, uint64_t>*> EdgeTrie::query(uint64_t prefix) {
    // Check if prefix exists
    auto lower = lower_bound(prefix_lvl.begin(), prefix_lvl.end(), prefix);
    bool found = lower != prefix_lvl.end() && *lower == prefix;
    auto begin_ptr = &data_lvl[0];

    // Return empty iterator if prefix is not in the tree
    if (!found) {
        return make_pair(begin_ptr, begin_ptr);
    }

    // Return range of results
    auto prefix_idx = lower - prefix_lvl.begin();
    return make_pair(begin_ptr + offsets[prefix_idx], begin_ptr + offsets[prefix_idx + 1]);
}


void EdgeTrie::print() {
    cout << "Edge Trie: \n" << endl;

    // Prefix level
    cout << "L0: ";
    for (size_t i = 0; i < prefix_lvl.size(); i++) {
        cout << prefix_lvl[i] << ",";
    }
    cout << endl;

    // Data level
    auto offset_idx = 1;
    cout << "L1: *";
    for (size_t i = 0; i < data_lvl.size(); i++) {
        cout << "(" << data_lvl[i].first << " e" << data_lvl[i].second << ")";
        if (i + 1 == offsets[offset_idx]) {
            offset_idx++;
            cout << "*";
        } else {
            cout << ",";
        }
    }
    cout << "\n" << endl;
}


void EdgeTrie::print_offsets() {
    // Offsets
    cout << "Offsets: \n" << endl;
    for (size_t i = 0; i < prefix_lvl.size(); i++) {
        cout << prefix_lvl[i] << ": (" << offsets[i] << "," << offsets[i + 1] << ")" << endl;
    }
    cout << endl;
}
