#include "trie_iter.h"

#include "storage/index/text_search/trie_node.h"
#include "storage/index/text_search/trie.h"
#include "storage/buffer_manager.h"

uint64_t TrieIter::next() {
    while (!trie_stack.empty()) {
begin_while:
        auto& trie_node = *trie_stack.top().trie_node;

        int N = *trie_node.child_count;
        int L = *trie_node.str_length;
        uint64_t V = trie_node.read_xbytes(trie_node.leaf_count, TrieNode::NP_SV);
        uint64_t info_value = trie_node.read_xbytes(trie_node.information, TrieNode::NP_SI);
        unsigned char* pos_info = trie_node.get_info_page_and_pos_info(&trie_node.info_page, info_value);

        // Leaves of current node
        for (auto& i = trie_stack.top().current_leaf; i < V; ) {
            uint64_t id = trie_node.read_xbytes(
                pos_info + N*(TrieNode::IP_SCh+TrieNode::IP_SO) + L + i*TrieNode::IP_SV,
                TrieNode::IP_SV
            );
            i++;
            return id;
        }

        // If exact_search we only need the leaves of one node and not the children
        if (exact_search) {
            trie_stack.pop();
            return 0;
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

            trie_stack.push({ std::make_unique<TrieNode>(trie_node.trie, next_node_page, next_node_offset), 0, 0 });

            i++;
            goto begin_while;
        }

        trie_stack.pop();
    }
    return 0;
}
