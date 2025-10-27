#include "trie.h"

#include <algorithm>

#include "storage/index/text_search/text_index.h"
#include "storage/index/text_search/trie_iter_list.h"
#include "storage/index/text_search/trie_iter_search.h"
#include "storage/index/text_search/trie_node.h"
#include "storage/index/text_search/utils.h"
#include "storage/page/page.h"
#include "system/buffer_manager.h"
#include "system/file_manager.h"

namespace TextSearch {

Trie::Trie(const std::filesystem::path& path) :
    file_id(file_manager.get_file_id(path)),
    garbage(file_manager.get_file_id(path.parent_path() / TextIndex::TRIE_GARBAGE_FILENAME))
{
    auto& first_page = buffer_manager.get_page_readonly(file_id, 0);

    auto* end_page_pointer = reinterpret_cast<uint64_t*>(first_page.get_bytes());

    if (end_page_pointer == 0) {
        // this is a new trie
        auto& first_page_editable = buffer_manager.get_page_readonly(file_id, 0);
        end_page_pointer = reinterpret_cast<uint64_t*>(first_page_editable.get_bytes());

        *end_page_pointer = ROOT_ID + Node::MAX_NODE_SIZE;
        buffer_manager.unpin(first_page_editable);
    }

    buffer_manager.unpin(first_page);
}

uint64_t Trie::get_new_id()
{
    auto& first_page = buffer_manager.get_page_editable(file_id, 0);
    auto* end_page_pointer = reinterpret_cast<uint64_t*>(first_page.get_bytes());
    auto* id_count = end_page_pointer + 1;
    auto res = *id_count;

    buffer_manager.unpin(first_page);
    return res;
}

uint64_t Trie::insert_string(const std::string& str)
{
    Node root_node(*this, ROOT_ID);
    return root_node.insert_string(nullptr, 0, reinterpret_cast<const uint8_t*>(str.c_str()));
}

TrieIterList Trie::get_iter_list()
{
    return TrieIterList(std::make_unique<Node>(*this, ROOT_ID));
}

template<SearchType type, bool allow_errors>
std::unique_ptr<TrieIter> Trie::search(const std::string& query)
{
    return std::make_unique<TrieIterSearch<type, allow_errors>>(
        std::make_unique<Node>(*this, ROOT_ID),
        query
    );
}

std::pair<uint64_t, uint64_t> Trie::get_space(uint64_t& capacity)
{
    // Make sure capacity is a power of 2
    capacity = std::pow(2, std::ceil(std::log2(capacity)));

    // First check garbage for available free space
    uint64_t node_pos;
    if (garbage.search_and_pop_capacity(capacity, node_pos)) {
        // Found free space in garbage
        return { node_pos / Page::SIZE, node_pos % Page::SIZE };
    }

    auto& first_page = buffer_manager.get_page_editable(file_id, 0);
    auto* end_page_pointer = reinterpret_cast<uint64_t*>(first_page.get_bytes());

    // Check if the space fits in the remainder of the last page
    node_pos = *end_page_pointer;
    auto available_space = Page::SIZE - (node_pos % Page::SIZE);
    if (available_space < capacity) {
        // Not enough space in the remainder of page.
        // Add it to the garbage if it is useful.
        if (available_space >= INITIAL_CAPACITY) {
            garbage.add_capacity(available_space, node_pos);
        }

        node_pos += available_space;
    }

    // Update end_page_pointer
    *end_page_pointer = node_pos + capacity;

    buffer_manager.unpin(first_page);
    return { node_pos / Page::SIZE, node_pos % Page::SIZE };
}

void Trie::print_trie(std::ostream& os, std::vector<std::string>&& text)
{
    os << "strict digraph {\n";

    if (!text.empty()) {
        std::sort(text.begin(), text.end());
        std::string label;

        std::string previous = text[0];
        size_t count = 1;

        for (size_t i = 1; i < text.size(); i++) {
            if (text[i] == previous) {
                count++;
            } else {
                label += escape(previous);
                if (count > 1) {
                    label += " x" + std::to_string(count);
                }
                label += "\\l";

                previous = text[i];
                count = 1;
            }
        }

        label += escape(previous);
        if (count > 1) {
            label += " x" + std::to_string(count);
        }
        label += "\\l";

        os << "labelloc=t; labeljust=l; label=\"" << label << "\"\n";
    }

    Node root_node(*this, ROOT_ID);
    root_node.print_trie_node(os);
    os << "}\n";
}

template std::unique_ptr<TrieIter> Trie::search<SearchType::MATCH, false>(const std::string&);
template std::unique_ptr<TrieIter> Trie::search<SearchType::MATCH, true>(const std::string&);
template std::unique_ptr<TrieIter> Trie::search<SearchType::PREFIX, false>(const std::string&);
template std::unique_ptr<TrieIter> Trie::search<SearchType::PREFIX, true>(const std::string&);

} // namespace TextSearch
