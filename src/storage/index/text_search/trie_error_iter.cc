#include "trie_error_iter.h"

#include "storage/index/text_search/trie.h"
#include "storage/index/text_search/trie_node.h"
#include "storage/buffer_manager.h"
#include "storage/file_manager.h"
#include "storage/page.h"


void TrieErrorIter::begin(const unsigned char* str, std::unique_ptr<TrieNode> root, int max_errors) {
    enumerating = false;
    enumerator = TrieErrorSubIter();
    this->max_errors = max_errors;

    pqueue.push(std::make_unique<SearchState>(std::move(root), 0, 0, 0, str));
}


uint64_t TrieErrorIter::next() {
beginning:
    if (enumerating) {
        auto id = enumerator.next();
        if (id != 0) { return id; }
        else { enumerating = false; }
    }

    while (pqueue.size() > 0) {
        // Get top SearchState
        auto& state = *pqueue.top();
        auto top_state = SearchState(std::move(state.trie_node), state.str_node_pos, state.hit_count,
                                     state.error_count, state.str);
        pqueue.pop();

        // Gather the data of the node:
        auto& node = top_state.trie_node;
        int N = *(node->child_count);
        int L = *(node->str_length);
        uint64_t info_value = TrieNode::read_xbytes(node->information, TrieNode::NP_SI);
        unsigned char* pos_info = node->get_info_page_and_pos_info(&node->info_page, info_value);

        // If the string is finished, we need to enumerate results
        if (!(*top_state.str)) {
            // Enumerates if score is >= 0
            if (top_state.score() >= 0) {
                enumerating = true;

                // Update error count with the remaining of str_node
                int current_errors = top_state.error_count + L - top_state.str_node_pos;

                if (exact_search && current_errors > max_errors) {
                    // Return empty subiter
                    enumerator = TrieErrorSubIter();
                }
                else {
                    enumerator = TrieErrorSubIter(top_state.trie_node->clone(), current_errors, this);
                }

                goto beginning;
            }
            continue;
        }

        // If the node has an associated string
        if (L > 0) {
            unsigned char* str_node = pos_info + N*(TrieNode::IP_SCh+TrieNode::IP_SO);
            str_node += top_state.str_node_pos;
            while (*(top_state.str) == *str_node && top_state.str_node_pos < L) {
                top_state.str++;
                str_node++;
                top_state.str_node_pos++;
                top_state.hit_count++;
            }

            // If the string is finished, we need to enumerate results
            if (!(*top_state.str)) {
                // Enumerates if score is >= 0
                if (top_state.score() >= 0) {
                    enumerating = true;

                    // Update error count with the remaining of str_node
                    int current_errors = top_state.error_count + L - top_state.str_node_pos;

                    if (exact_search && current_errors > max_errors) {
                        // Return empty subiter
                        enumerator = TrieErrorSubIter();
                    }
                    else {
                        enumerator = TrieErrorSubIter(top_state.trie_node->clone(), current_errors, this);
                    }

                    goto beginning;
                }
                continue;
            }

            if (top_state.str_node_pos < L) {
                // Check if we have reached the error limit
                if (top_state.error_count + 1 > max_errors) { continue; }

                // Check variants
                // 1) Skip current letter
                pqueue.push(std::make_unique<SearchState>(
                    top_state.trie_node->clone(),
                    top_state.str_node_pos,
                    top_state.hit_count,
                    top_state.error_count + 1,
                    top_state.str + 1));

                // 2) Current letter is not in the correct position
                pqueue.push(std::make_unique<SearchState>(
                    top_state.trie_node->clone(),
                    top_state.str_node_pos + 1,
                    top_state.hit_count,
                    top_state.error_count + 1,
                    top_state.str));

                // 3) Both cases above
                pqueue.push(std::make_unique<SearchState>(
                    top_state.trie_node->clone(),
                    top_state.str_node_pos + 1,
                    top_state.hit_count,
                    top_state.error_count + 1,
                    top_state.str + 1));

                continue;
            }
        }

        // Search next child
        int found = 0;
        for (int i = 0; i < N; i++) {
            if (*(top_state.str) == *pos_info) { found = 1; break; }
            pos_info += TrieNode::IP_SCh + TrieNode::IP_SO;
        }

        // If found, add to priority queue
        if (found == 1) {
            uint64_t next_node_pos = TrieNode::read_xbytes(pos_info + TrieNode::IP_SCh, TrieNode::IP_SO);
            uint64_t next_node_offset = next_node_pos % node->trie.PAGE_SIZE;
            uint64_t next_node_page_num = next_node_pos / node->trie.PAGE_SIZE;
            auto& next_node_page = buffer_manager.get_page(node->trie.node_file_id, next_node_page_num);
            TrieNode next_node(node->trie, next_node_page, next_node_offset);

            pqueue.push(std::make_unique<SearchState>(
                next_node.clone(),
                0,
                top_state.hit_count + 1,
                top_state.error_count,
                top_state.str + 1));
        }

        // Check if we have reached the error limit
        if (top_state.error_count + 1 > max_errors) { continue; }

        // Check variants
        // 1) Skip current letter
        pqueue.push(std::make_unique<SearchState>(
            top_state.trie_node->clone(),
            top_state.str_node_pos,
            top_state.hit_count,
            top_state.error_count + 1,
            top_state.str + 1));

        // Get next nodes
        pos_info = node->get_info_page_and_pos_info(&node->info_page, info_value);
        for (int i = 0; i < N; i++) {
            uint64_t next_node_pos = TrieNode::read_xbytes(pos_info + TrieNode::IP_SCh, TrieNode::IP_SO);
            uint64_t next_node_offset = next_node_pos % node->trie.PAGE_SIZE;
            uint64_t next_node_page_num = next_node_pos / node->trie.PAGE_SIZE;
            auto& next_node_page = buffer_manager.get_page(node->trie.node_file_id, next_node_page_num);
            TrieNode next_node(node->trie, next_node_page, next_node_offset);

            // 2) Current letter is not in the correct position
            pqueue.push(std::make_unique<SearchState>(
                next_node.clone(),
                0,
                top_state.hit_count,
                top_state.error_count + 1,
                top_state.str));

            // 3) Both cases above
            pqueue.push(std::make_unique<SearchState>(
                next_node.clone(),
                0,
                top_state.hit_count,
                top_state.error_count + 1,
                top_state.str + 1));

            pos_info += TrieNode::IP_SCh + TrieNode::IP_SO;
        }
    }

    return 0;
}


uint64_t TrieErrorSubIter::next() {
    while (!trie_stack.empty()) {
begin_while:
        auto& trie_node = *trie_stack.top().trie_node;

        // Get global offset of node (id) to check duplicates
        auto node_pos_in_page = reinterpret_cast<char*>(trie_node.child_count) - trie_node.node_page.get_bytes();
        uint64_t global_offset = node_pos_in_page + trie_node.node_page.get_page_number() * trie_node.trie.PAGE_SIZE;

        // Check duplicates
        bool duplicated = false;
        if (main_iter->duplicates.find(global_offset) != main_iter->duplicates.end()) {
            duplicated = true;
        }

        // Gather the data of the node:
        int N = *trie_node.child_count;
        int L = *trie_node.str_length;
        uint64_t V = trie_node.read_xbytes(trie_node.leaf_count, TrieNode::NP_SV);
        uint64_t info_value = trie_node.read_xbytes(trie_node.information, TrieNode::NP_SI);
        unsigned char* pos_info = trie_node.get_info_page_and_pos_info(&trie_node.info_page, info_value);

        // Leaves of current node
        if (!duplicated) {
            for (auto& i = trie_stack.top().current_leaf; i < V; ) {
                uint64_t id = trie_node.read_xbytes(
                    pos_info + N*(TrieNode::IP_SCh+TrieNode::IP_SO) + L + i*TrieNode::IP_SV,
                    TrieNode::IP_SV
                );
                i++;
                return id;
            }
        }

        // Add children of node to stack
        for (auto& i = trie_stack.top().current_children; i < N; ) {
            uint64_t next_node_pos = trie_node.read_xbytes(
                pos_info + TrieNode::IP_SCh + i*(TrieNode::IP_SCh+TrieNode::IP_SO),
                TrieNode::IP_SO
            );
            uint64_t next_node_offset = next_node_pos % trie_node.trie.PAGE_SIZE;
            uint64_t next_node_page_num = next_node_pos / trie_node.trie.PAGE_SIZE;
            auto& next_node_page = buffer_manager.get_page(trie_node.trie.node_file_id, next_node_page_num);
            std::unique_ptr<TrieNode> next_node = std::make_unique<TrieNode>(trie_node.trie, next_node_page,
                                                                             next_node_offset);

            // Update error count with every new character of the next_node
            int L_next_node = *next_node->str_length;
            int current_errors = trie_stack.top().current_errors + 1 + L_next_node;

            // If exact_search and number of errors > max_errors, do not add to stack
            if (main_iter->exact_search && current_errors > main_iter->max_errors) {
                i++;
                continue;
            }
            trie_stack.push({ std::move(next_node), current_errors, 0, 0 });

            i++;
            goto begin_while;
        }

        if (!duplicated) { main_iter->duplicates.insert(global_offset); }
        trie_stack.pop();
    }
    return 0;
}