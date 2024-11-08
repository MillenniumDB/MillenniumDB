#pragma once

#include <memory>
#include <queue>
#include <stack>
#include <string>
#include <unordered_set>

#include "storage/index/text_search/search_type.h"
#include "storage/index/text_search/trie_iter.h"

namespace TextSearch {

class Node;

// A search not permitting errors uses max_errors = 0.
// However, for efficiency reason all the checks are of the form (allow_errors && error_count < max_errors).
// This way the condition can short circuit during compilation time when allow_errors is false and can
// potentially be optimized out by the compiler.
template<SearchType type, bool allow_errors>
class TrieIterSearch : public TrieIter {
public:
    TrieIterSearch(std::unique_ptr<Node> trie_node,const std::string& query_, uint8_t max_errors = 1) : max_errors(max_errors) {
        priority_queue.push({std::move(trie_node), 0, 0, 0, 0});

        if (!allow_errors) {
            max_errors = 0;
        }
        query = std::basic_string<unsigned char>(query_.data(), query_.data() + query_.size());
    }

    // Returns true if a node was found
    bool next() override;

    // Called when next() returns true to get the node_id of the current node
    uint64_t get_node_id() override {
        return node_id;
    }

private:
    struct SearchState {
        std::unique_ptr<Node> node;         // The current trie node
        uint16_t              query_idx;    // The current index of the query string (match/prefix)
        uint16_t              node_str_idx; // The current index of the string of the node
        uint16_t              matched_len;  // Length of the string from trie root to current node
        uint8_t               error_count;  // The current number of errors made

        SearchState(std::unique_ptr<Node> node,
                    uint16_t              query_idx,
                    uint16_t              node_str_idx,
                    uint16_t              matched_len,
                    uint8_t               error_count) :
            node         (std::move(node)),
            query_idx    (query_idx),
            node_str_idx (node_str_idx),
            matched_len  (matched_len),
            error_count  (error_count) { }
    };

    void maybe_add_search_state(std::unique_ptr<Node>&& node,
                                uint16_t                query_idx,
                                uint16_t                node_str_idx,
                                uint16_t                matched_len,
                                uint8_t                 error_count);

    // Used to prioritize the search states when added to the priority queue
    struct CompareSearchStates {
        bool operator()(const SearchState& left, const SearchState& right) const {
            if (allow_errors && left.error_count != right.error_count) {
                return left.error_count > right.error_count;
            }
            return left.matched_len > right.matched_len;
        }
    };

    // Holds the next search states to check, ordered by priority
    std::priority_queue<SearchState, std::vector<SearchState>, CompareSearchStates> priority_queue;

    // Already returned node identifiers to avoid duplicates
    std::unordered_set<uint64_t> returned_node_ids;

    // The match/prefix we are searching for, does not include null terminator
    std::basic_string<unsigned char> query;

    // The node_id of the found node, set when next() returns true
    uint64_t node_id;

    // Maximum number of errors allowed to commit
    uint8_t max_errors;
};

} // namespace TextSearch
