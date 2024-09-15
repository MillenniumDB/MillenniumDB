#include "trie_iter_search.h"

#include "storage/index/text_search/trie_node.h"
#include "storage/index/text_search/utils.h"


namespace TextSearch {


static uint16_t inc(uint16_t val, unsigned char c) {
    // Check if c is a continuation byte (0b10 in most significant position)
    // and only increment if a is not a continuation byte
    return (c & 0b1100'0000) == 0b1000'0000 ? val : val + 1;
}


static uint16_t inc(uint16_t val, unsigned char a, unsigned char b) {
    // Check if both a and b are continuation bytes (0b10 in most significant position)
    // and increment if a or b is a continuation byte
    return (a & 0b1100'0000) == 0b1000'0000
        && (b & 0b1100'0000) == 0b1000'0000 ? val : val + 1;
}


template <SearchType type, bool allow_errors>
void TrieIterSearch<type, allow_errors>::maybe_add_search_state(
    std::unique_ptr<Node>&& node,
    uint16_t                query_idx,
    uint16_t                node_str_idx,
    uint16_t                matched_len,
    uint8_t                 error_count)
{
    if (allow_errors && error_count <= max_errors) {
        priority_queue.emplace(std::move(node), query_idx, node_str_idx, matched_len, error_count);
    }
}


template <SearchType type, bool allow_errors>
bool TrieIterSearch<type, allow_errors>::next() {
    while (!priority_queue.empty()) {
        // Remove const and move out the node so that we can pop the state
        auto node = std::move(const_cast<std::unique_ptr<Node>&>(priority_queue.top().node));
        auto const query_idx = priority_queue.top().query_idx;
        auto const node_str_idx = priority_queue.top().node_str_idx;
        auto const matched_len = priority_queue.top().matched_len;
        auto const error_count = priority_queue.top().error_count;

        priority_queue.pop();

        auto node_str_length = static_cast<size_t>(*node->str_len_ptr);

        if (query_idx < query.size() || node_str_idx < node_str_length) {
            assert(query_idx <= query.size());
            assert(node_str_idx <= node_str_length);

            if (query_idx >= query.size())
            {
                // We have reached the end of the prefix
                if constexpr (type == SearchType::Match) {
                    if (allow_errors) {
                        // We are still allowed to make errors
                        auto next_error_count = inc(error_count, node->string_ptr[node_str_idx]);
                        auto next_matched_len = inc(matched_len, node->string_ptr[node_str_idx]);
                        maybe_add_search_state(node->clone(), query_idx, node_str_idx + 1, next_matched_len, next_error_count);
                    }
                }
                if constexpr (type == SearchType::Prefix) {
                    auto next_matched_len = inc(matched_len, node->string_ptr[node_str_idx]);
                    priority_queue.emplace(node->clone(), query_idx, node_str_idx + 1, next_matched_len, error_count);
                }
            }
            else if (node_str_idx >= node_str_length)
            {
                // We have reached the end of the string of the node

                // Add children to priority_queue
                for (size_t c = 0; c < *node->child_count_ptr; c++) {
                    auto child_ptr = node->children_ptr + (c * Node::CHILD_SIZE);
                    auto child_char = *child_ptr;
                    auto child_page_pointer = read_bytes(child_ptr + Node::CHILD_CHAR_SIZE, Node::CHILD_POINTER_SIZE);

                    if (child_char == query[query_idx]) {
                        // The char of the edge to child matches the prefix
                        auto child_node = std::make_unique<Node>(node->trie, child_page_pointer);
                        auto next_matched_len = inc(matched_len, child_char);
                        priority_queue.emplace(std::move(child_node), query_idx + 1, 0, next_matched_len, error_count);
                    } else if (allow_errors) {
                        // The char of the edge to child does not match the prefix
                        // However, we are still allowed to make errors
                        auto child_node = std::make_unique<Node>(node->trie, child_page_pointer);
                        auto next_error_count = inc(error_count, child_char);
                        auto next_matched_len = inc(matched_len, child_char);
                        maybe_add_search_state(child_node->clone(), query_idx, 0, next_matched_len, next_error_count);

                        next_error_count = inc(error_count, child_char, query[query_idx]);
                        next_matched_len = inc(matched_len, child_char);
                        maybe_add_search_state(std::move(child_node), query_idx + 1, 0, next_matched_len, next_error_count);
                    }
                }

                if (allow_errors) {
                    // We are still allowed to make errors
                    auto next_error_count = inc(error_count, query[query_idx]);
                    maybe_add_search_state(node->clone(), query_idx + 1, node_str_idx, matched_len, next_error_count);
                }
            }
            else if (query[query_idx] != node->string_ptr[node_str_idx])
            {
                // Non matching chars
                if (allow_errors) {
                    // We are still allowed to make errors
                    // Add all three alternatives
                    auto next_error_count = inc(error_count, query[query_idx]);
                    maybe_add_search_state(node->clone(), query_idx + 1, node_str_idx, matched_len, next_error_count);

                    next_error_count = inc(error_count, node->string_ptr[node_str_idx]);
                    auto next_matched_len = inc(matched_len, node->string_ptr[node_str_idx]);
                    maybe_add_search_state(node->clone(), query_idx, node_str_idx + 1, next_matched_len, next_error_count);

                    next_error_count = inc(error_count, node->string_ptr[node_str_idx], query[query_idx]);
                    next_matched_len = inc(matched_len, node->string_ptr[node_str_idx]);
                    maybe_add_search_state(std::move(node), query_idx + 1, node_str_idx + 1, next_matched_len, next_error_count);
                }

            }
            else
            {
                // Matching chars
                auto next_matched_len = inc(matched_len, node->string_ptr[node_str_idx]);
                priority_queue.emplace(node->clone(), query_idx + 1, node_str_idx + 1, next_matched_len, error_count);
            }
        } else {
            // At this point we have consumed the prefix and the string of this node
            // Check if we still have errors left
            if (type == SearchType::Prefix || allow_errors) {
                for (size_t c = 0; c < *node->child_count_ptr; c++) {
                    auto child_ptr          = node->children_ptr + (c * Node::CHILD_SIZE);
                    auto child_char         = *child_ptr;
                    auto child_page_pointer = read_bytes(child_ptr + Node::CHILD_CHAR_SIZE, Node::CHILD_POINTER_SIZE);
                    auto child_node         = std::make_unique<Node>(node->trie, child_page_pointer);

                    if constexpr (type == SearchType::Match) {
                        auto next_error_count = inc(error_count, child_char);
                        auto next_matched_len = inc(matched_len, child_char);
                        maybe_add_search_state(std::move(child_node), query_idx, 0, next_matched_len, next_error_count);
                    }
                    if constexpr (type == SearchType::Prefix) {
                        auto next_matched_len = inc(matched_len, child_char);
                        priority_queue.emplace(std::move(child_node), query_idx, 0, next_matched_len, error_count);
                    }
                }
            }

            auto document_count = read_bytes(node->document_count_ptr, Node::DOCUMENT_COUNT_SIZE);
            if (document_count > 0) {
                // We have found a match
                node_id = read_bytes(node->node_id_ptr, Node::NODE_ID_SIZE);
                if (returned_node_ids.insert(node_id).second) {
                    return true;
                }
            }
        }
    }
    return false;
}


template class TrieIterSearch<SearchType::Match , true >;
template class TrieIterSearch<SearchType::Match , false>;
template class TrieIterSearch<SearchType::Prefix, true >;
template class TrieIterSearch<SearchType::Prefix, false>;


} // namespace TextSearch
