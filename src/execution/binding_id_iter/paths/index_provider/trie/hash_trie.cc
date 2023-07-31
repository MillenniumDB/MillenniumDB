#include <iostream>
#include <map>

#include "hash_trie.h"

using namespace std;


void HashTrie::insert(uint64_t prefix, uint64_t data) {
    // Check if prefix is already in the tree
    bool new_prefix = prefix != last_prefix;

    // Add new prefix
    if (new_prefix) {
        // Store last prefix + offset
        offsets.insert({last_prefix, make_pair(last_offset, last_offset + data_count)});
        last_offset += data_count;
        data_count = 0;

        // New prefix being inserted
        last_prefix = prefix;
    }

    // Store data
    data_lvl.push_back(data);
    data_count++;
}


void HashTrie::end_inserts() {
    // Store last prefix + offset
    offsets.insert({last_prefix, make_pair(last_offset, last_offset + data_count)});
}


pair<uint64_t*, uint64_t*> HashTrie::query(uint64_t prefix) {
    // Check if prefix exists
    auto data_offset = offsets.find(prefix);
    bool found = data_offset != offsets.end();
    auto begin_ptr = &data_lvl[0];

    // Return empty iterator if prefix is not in the tree
    if (!found) {
        return make_pair(begin_ptr, begin_ptr);
    }

    // Return range of results
    return make_pair(begin_ptr + data_offset->second.first, begin_ptr + data_offset->second.second);
}


void HashTrie::print() {
    cout << "Hash Trie: \n" << endl;

    // Generate ordered map for printing
    auto prefix_offsets = map<uint64_t, pair<uint32_t, uint32_t>>();
    for (const auto& e : offsets) {
        prefix_offsets.insert({e.first, e.second});
    }

    // Prefix level
    cout << "L0: ";
    for (const auto& prefix : prefix_offsets) {
        if (prefix.first != invalid_object_id) {
            cout << prefix.first << ",";
        }
    }
    cout << endl;

    // Data level
    auto offset_data = prefix_offsets.begin();
    cout << "L1: *";
    for (size_t i = 0; i < data_lvl.size(); i++) {
        cout << data_lvl[i];
        if (i + 1 == offset_data->second.second) {
            offset_data++;
            cout << "*";
        } else {
            cout << ",";
        }
    }
    cout << "\n" << endl;
}


void HashTrie::print_offsets() {
    // Generate ordered map for printing
    auto prefix_offsets = map<uint64_t, pair<uint32_t, uint32_t>>();
    for (const auto& e : offsets) {
        prefix_offsets.insert({e.first, e.second});
    }

    // Offsets
    cout << "Offsets: \n" << endl;
    for (const auto& prefix : prefix_offsets) {
        if (prefix.first != invalid_object_id) {
            cout << prefix.first << ": (" << prefix.second.first << "," << prefix.second.second << ")" << endl;
        }
    }
    cout << endl;
}
