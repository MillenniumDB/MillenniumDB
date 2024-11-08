#include "trie.h"

using namespace std;


void Trie::insert(uint64_t prefix, uint64_t data) {
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
    data_lvl.push_back(data);
    data_count++;
}


void Trie::end_inserts() {
    // Store last offset
    offsets.push_back(last_offset + data_count);
}


pair<uint64_t*, uint64_t*> Trie::query(uint64_t prefix) {
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


// void Trie::print(std::ostream& os) {
//     os << "Trie: \n" << endl;

//     // Prefix level
//     os << "L0: ";
//     for (size_t i = 0; i < prefix_lvl.size(); i++) {
//         os << prefix_lvl[i] << ",";
//     }
//     os << endl;

//     // Data level
//     auto offset_idx = 1;
//     os << "L1: *";
//     for (size_t i = 0; i < data_lvl.size(); i++) {
//         os << data_lvl[i];
//         if (i + 1 == offsets[offset_idx]) {
//             offset_idx++;
//             os << "*";
//         } else {
//             os << ",";
//         }
//     }
//     os << "\n" << endl;
// }


// void Trie::print_offsets(std::ostream& os) {
//     // Offsets
//     os << "Offsets: \n" << endl;
//     for (size_t i = 0; i < prefix_lvl.size(); i++) {
//         os << prefix_lvl[i] << ": (" << offsets[i] << "," << offsets[i + 1] << ")" << endl;
//     }
//     os << endl;
// }
