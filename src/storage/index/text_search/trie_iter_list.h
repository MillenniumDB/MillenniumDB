#pragma once

#include <cstdint>
#include <memory>
#include <stack>
#include <string>

namespace TextSearch {

class Node;

class TrieIterList {
public:
    TrieIterList(std::unique_ptr<Node> trie_node) {
        stack.push({ std::move(trie_node), -1 });
    }

    // Returns true if there are more strings
    bool next();

    // Returns a reference of the current string.
    // Should be called after next() return true.
    const std::string& get_str() {
        return string;
    }

    // Return the node_id of the curren node.
    // Should be called after next() returns true.
    uint64_t get_node_id() {
        return node_id;
    }

private:
    // Represents the iteration state
    struct State {
        std::unique_ptr<Node> node; // Node associated with the state
        int16_t current_child;      // Next child to select, -1 means this nodes, >=0 means child
    };

    // Holds a branch of the current trie traversal
    std::stack<State> stack;

    // The string represented by the current node
    std::string string;

    // The node id of the current node
    uint64_t node_id;
};

} // namespace TextSearch
