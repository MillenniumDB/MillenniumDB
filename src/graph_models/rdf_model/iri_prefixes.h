#pragma once

#include <array>
#include <cassert>
#include <cstdint>
#include <set>
#include <string>
#include <vector>


constexpr uint_fast8_t IRI_PREFIX_CHAR_MIN = 0x21;
constexpr uint_fast8_t IRI_PREFIX_CHAR_MAX = 0x7E;

class IriPrefixes {
private:
    // Each vector element represents a node of a prefix trie.
    // The index of the element is the node identifier.
    // Each array is a transition table from ASCII values (in the range CHAR_MIN - CHAR_MAX) to a child node.
    std::vector<std::array<uint_fast16_t, IRI_PREFIX_CHAR_MAX - IRI_PREFIX_CHAR_MIN + 1>> transitions;

    // Hold the longest prefix seen at each node of the trie
    std::vector<uint_fast8_t> longest_prefix;

    std::vector<std::string> prefixes;

public:
    // init() has to be called before using this class
    void init(std::set<std::string>&& prefix_set);

    void init(std::vector<std::string>&& prefix_set);

    // Get the id and size of the prefix (possibly the empty prefix)
    std::pair<uint_fast8_t, size_t> get_prefix_id(const char* string, size_t size) const;

    // Get the id and size of the prefix (possibly the empty prefix)
    std::pair<uint_fast8_t, size_t> get_prefix_id(const std::string& string) const {
        return get_prefix_id(string.data(), string.size());
    }

    const std::string& get_prefix(uint_fast8_t id) const {
        assert(id < prefixes.size());
        return prefixes[id];
    }

    const std::vector<std::string>& get_prefix_list() const {
        return prefixes;
    }
};