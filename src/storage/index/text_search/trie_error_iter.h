#pragma once

#include <memory>
#include <vector>
#include <queue>
#include <set>
#include <stack>

#include "storage/index/text_search/trie_iter.h"

class TrieNode;
class TrieErrorIter;

class TrieErrorSubIter {
public:
    TrieErrorSubIter(std::unique_ptr<TrieNode> trie_node, int current_errors, TrieErrorIter* iter) : 
        main_iter (iter)
    {
        trie_stack.push({ std::move(trie_node), current_errors, 0, 0 });
    }

    // for empty iter
    TrieErrorSubIter() { }

    // if returns 0 it means there is no next
    uint64_t next();

private:
    struct NodeInfo {
        std::unique_ptr<TrieNode> trie_node;
        int current_errors;
        int current_children;
        uint64_t current_leaf;
    };
    std::stack<NodeInfo> trie_stack;

    TrieErrorIter* main_iter;
};


class TrieErrorIter {
friend class TrieErrorSubIter;
public:
    TrieErrorIter(bool exact_search) { this->exact_search = exact_search; }

    void begin(const unsigned char* str, std::unique_ptr<TrieNode> root, int max_errors);

    // If it returns 0 it means that there is no next
    uint64_t next();

private:
    struct SearchState {
        mutable std::unique_ptr<TrieNode> trie_node;
        int str_node_pos;
        int hit_count;
        int error_count;
        const unsigned char* str;

        int score() { return hit_count - error_count; }

        struct Compare {
            bool operator()(const std::unique_ptr<SearchState>& left, const std::unique_ptr<SearchState>& right) const {
                return left->score() < right->score();
            }
        };

        SearchState(std::unique_ptr<TrieNode> trie_node, int str_node_pos, int hit_count,
                    int error_count, const unsigned char* str) :
            trie_node (std::move(trie_node)),
            str_node_pos (str_node_pos),
            hit_count (hit_count),
            error_count (error_count),
            str (str) {}
    };
    std::priority_queue<std::unique_ptr<SearchState>, std::vector<std::unique_ptr<SearchState>>, SearchState::Compare> pqueue;
    bool enumerating;
    TrieErrorSubIter enumerator;
    int max_errors;
    std::set<uint64_t> duplicates;
    bool exact_search;
};
