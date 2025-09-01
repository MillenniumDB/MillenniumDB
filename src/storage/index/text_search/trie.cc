#include "trie.h"

#include <algorithm>
#include <cassert>

#include "storage/index/text_search/text_index.h"
#include "storage/index/text_search/trie_iter_list.h"
#include "storage/index/text_search/trie_iter_search.h"
#include "storage/index/text_search/trie_node.h"
#include "storage/index/text_search/utils.h"
#include "storage/page/versioned_page.h"
#include "system/buffer_manager.h"
#include "system/file_manager.h"

namespace TextSearch {

Trie::Trie(bool create_new, const std::filesystem::path& path) : file_id(file_manager.get_file_id(path))
{
    const auto garbage_relative_path = path.parent_path() / TextIndex::TRIE_GARBAGE_FILENAME;
    if (create_new) {
        garbage = TrieGarbage::create(garbage_relative_path);
        root_page = &buffer_manager.append_vpage(file_id);
    } else {
        garbage = TrieGarbage::load(garbage_relative_path);
        root_page = &buffer_manager.get_page_readonly(file_id, 0);
    }

    // end_page_pointer_ptr = reinterpret_cast<unsigned char*>(root_page->get_bytes());
    // root_page_pointer_ptr = end_page_pointer_ptr + PAGE_POINTER_SIZE;
    // next_id_ptr = root_page_pointer_ptr + PAGE_POINTER_SIZE;

    // Use load argument to check if we have to create a new trie or load an existing trie
    if (create_new) {
        // New trie, initialize end_page_pointer
        // write_bytes(end_page_pointer_ptr(), PAGE_POINTER_SIZE, HEADER_SIZE);
        set_end_page_pointer(HEADER_SIZE);
        root_node = std::make_unique<Node>(*this, nullptr, 0, 0, nullptr, 0);
        set_root_page_pointer(root_node->page_pointer());
        // write_bytes(root_page_pointer_ptr(), PAGE_POINTER_SIZE, root_node->page_pointer());
    } else {
        // Trie was already created, load root node
        // auto root_page_pointer = read_bytes(root_page_pointer_ptr(), PAGE_POINTER_SIZE);
        auto root_page_pointer = get_root_page_pointer();
        assert(root_page_pointer >= HEADER_SIZE);
        root_node = std::make_unique<Node>(*this, root_page_pointer);
    }
}

Trie::~Trie()
{
    buffer_manager.unpin(*root_page);
}

uint64_t Trie::insert_string(const std::string& str)
{
    return root_node->insert_string(nullptr, nullptr, reinterpret_cast<const unsigned char*>(str.c_str()));
}

TrieIterList Trie::get_iter_list()
{
    return TrieIterList(root_node->clone());
}

template<SearchType type, bool allow_errors>
std::unique_ptr<TrieIter> Trie::search(const std::string& query)
{
    return std::make_unique<TrieIterSearch<type, allow_errors>>(root_node->clone(), query);
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

    root_node->print_trie_node(os);
    os << "}\n";
}

template std::unique_ptr<TrieIter> Trie::search<SearchType::MATCH, false>(const std::string&);
template std::unique_ptr<TrieIter> Trie::search<SearchType::MATCH, true>(const std::string&);
template std::unique_ptr<TrieIter> Trie::search<SearchType::PREFIX, false>(const std::string&);
template std::unique_ptr<TrieIter> Trie::search<SearchType::PREFIX, true>(const std::string&);

} // namespace TextSearch
