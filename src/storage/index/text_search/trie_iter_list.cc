#include "trie_iter_list.h"

#include "storage/index/text_search/trie.h"
#include "storage/index/text_search/trie_node.h"
#include "storage/index/text_search/utils.h"


namespace TextSearch {


bool TrieIterList::next() {
    start:
    while (!stack.empty()) {
        auto& node = *stack.top().node;
        auto& child_idx = stack.top().current_child;

        if (child_idx == -1) {
            child_idx++;
            string.append(node.string_ptr, node.string_ptr + *node.str_len_ptr);

            auto document_count = read_bytes(node.document_count_ptr, node.DOCUMENT_COUNT_SIZE);
            if (document_count > 0) {
                node_id = read_bytes(node.node_id_ptr, Node::NODE_ID_SIZE);
                return true;
            }
        }

        if (child_idx < *node.child_count_ptr) {
            auto child_char = node.children_ptr[child_idx * node.CHILD_SIZE ];
            auto child_page_pointer_ptr = &node.children_ptr[child_idx * node.CHILD_SIZE + node.CHILD_CHAR_SIZE];
            auto child_page_pointer = read_bytes(child_page_pointer_ptr, node.CHILD_POINTER_SIZE);
            auto child_node = std::make_unique<Node>(node.trie, child_page_pointer);

            string += child_char;
            stack.push({ std::move(child_node), -1 });

            child_idx++;
            goto start;
        }

        // The root node does not have an incoming edge with a char, only child nodes
        auto added_length = std::min(string.size(), static_cast<size_t>(*node.str_len_ptr) + 1);
        string.resize(string.size() - added_length);
        stack.pop();
    }
    return false;
}


} // namespace TextSearch
