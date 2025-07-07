#include "iri_prefixes.h"

#include <algorithm>

// Orders strings from shortest to longest,
// and same size strings lexicographically.
struct string_comparison {
    inline bool operator()(const std::string& first, const std::string& second) const {
        if (first.size() != second.size()) {
            return first.size() < second.size();
        } else {
            return first < second;
        }
    }
};


void IriPrefixes::init(std::set<std::string>&& prefix_set) {
    // There should be no reason to initialize twice
    assert(prefixes.size() == 0);

    // Make sure empty prefix is present
    prefix_set.emplace();

    assert(prefix_set.size() <= 256);

    prefixes.reserve(prefix_set.size());
    for (auto& prefix : prefix_set) {
        prefixes.push_back(std::move(prefix));
    }

    // The prefixes have to be in order of increasing size
    std::sort(prefixes.begin(), prefixes.end(), string_comparison());

    // node 0 is the root
    transitions.emplace_back(); // 0 initialized
    longest_prefix.emplace_back(); // 0 initialized

    for (size_t p = 0; p < prefixes.size(); p++) {
        // For each prefix start at the root node
        uint_fast16_t current_node = 0;

        for (unsigned char c : prefixes[p]) {
            assert(IRI_PREFIX_CHAR_MIN <= c && c <= IRI_PREFIX_CHAR_MAX);
            // c will be in range [0, CHAR_MAX - CHAR_MIN]
            c -= IRI_PREFIX_CHAR_MIN;
            auto next_node = transitions[current_node][c];

            if (next_node == 0) {
                // New node
                next_node = transitions.size();

                transitions.emplace_back(); // 0 initialized
                longest_prefix.emplace_back(); // 0 initialized

                transitions[current_node][c] = next_node;
            }

            if (longest_prefix[next_node] == 0) {
                // Set the longest prefix seen up to this point.
                // This works because we visit the prefixes in order of increasing size
                longest_prefix[next_node] = longest_prefix[current_node];
            }

            current_node = next_node;
        }
        // Prefix-id 0 is the empty prefix, by adding 1 we get a value > 0
        longest_prefix[current_node] = p;
    }

    // std::cout << "Compression Prefixes:\n";
    // for (auto& prefix : prefixes) {
    //     std::cout << " - \"" << prefix << "\"\n";
    // }
}


void IriPrefixes::init(std::vector<std::string>&& prefix_vec) {
    std::set<std::string> prefix_set;

    for (auto& prefix : prefix_vec) {
        prefix_set.insert(std::move(prefix));
    }

    init(std::move(prefix_set));
}


std::pair<uint_fast8_t, size_t> IriPrefixes::get_prefix_id(const char* string, size_t size) const { 
    uint_fast16_t current_node = 0;

    for (size_t i = 0; i < size; i++) {
        unsigned char c = string[i];

        if (c < IRI_PREFIX_CHAR_MIN || c > IRI_PREFIX_CHAR_MAX) {
            // c is outside of allowed prefix range
            break;
        }

        auto next_node = transitions[current_node][c - IRI_PREFIX_CHAR_MIN];

        if (next_node == 0) {
            // next_node is not part of trie.
            // Either current_node is a leaf_node,
            // or no prefix continues with the char c.
            break;
        } else {
            current_node = next_node;
        }
    }

    assert(longest_prefix[current_node] <= prefixes.size());

    auto prefix_id = longest_prefix[current_node];
    auto prefix_size = prefixes[prefix_id].size();

    return {prefix_id, prefix_size};
}
