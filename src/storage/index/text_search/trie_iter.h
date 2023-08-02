#pragma once

#include <memory>
#include <stack>

class TrieNode;

class TrieIter {
public:
    TrieIter(std::unique_ptr<TrieNode> trie_node, bool exact_search = false) {
        trie_stack.push({ std::move(trie_node), 0, 0 });
        this->exact_search = exact_search;
    }

    // for empty iter
    TrieIter() { }

    // if returns 0 it means there is no next
    uint64_t next();

private:
    struct NodeInfo {
        std::unique_ptr<TrieNode> trie_node;
        int current_children;
        uint64_t current_leaf;
    };
    std::stack<NodeInfo> trie_stack;

    bool exact_search;
};
