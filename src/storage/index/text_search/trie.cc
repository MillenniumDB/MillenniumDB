#include "trie.h"

#include <cassert>
#include <fstream>

#include "storage/index/text_search/trie_iter_list.h"
#include "storage/index/text_search/trie_iter_search.h"
#include "storage/index/text_search/trie_node.h"
#include "storage/index/text_search/utils.h"
#include "storage/page/unversioned_page.h"
#include "system/buffer_manager.h"
#include "system/file_manager.h"

namespace TextSearch {

Trie::Trie(std::filesystem::path path) :
    file_id   (file_manager.get_file_id(path)),
    garbage   (path),
    root_page (buffer_manager.get_unversioned_page(file_id, 0))
{
    // Initialize pointers
    end_page_pointer_ptr  = reinterpret_cast<unsigned char*>(root_page.get_bytes());
    root_page_pointer_ptr = end_page_pointer_ptr + PAGE_POINTER_SIZE;
    next_id_ptr           = root_page_pointer_ptr + PAGE_POINTER_SIZE;

    // Use end_page_pointer to check if we have to create a new tire or load an existing trie
    auto end_page_pointer = read_bytes(end_page_pointer_ptr, PAGE_POINTER_SIZE);
    if (end_page_pointer == 0) {
        // New trie, initialize end_page_pointer
        write_bytes(end_page_pointer_ptr, PAGE_POINTER_SIZE, HEADER_SIZE);
        // Create root node
        root_node = std::make_unique<Node>(*this, nullptr, 0, 0, nullptr, 0);
        write_bytes(root_page_pointer_ptr, PAGE_POINTER_SIZE, root_node->page_pointer());
        root_page.make_dirty();
    } else {
        // Trie was already created, load root node
        auto root_page_pointer = read_bytes(root_page_pointer_ptr, PAGE_POINTER_SIZE);
        assert (root_page_pointer >= HEADER_SIZE);
        root_node = std::make_unique<Node>(*this, root_page_pointer);
    }
}


Trie::~Trie() {
    buffer_manager.unpin(root_page);
}


uint64_t Trie::insert_string(const std::string& str) {
    return root_node->insert_string(nullptr, nullptr, reinterpret_cast<const unsigned char*> (str.c_str()));
}


TrieIterList Trie::get_iter_list() {
    return TrieIterList(root_node->clone());
}

template<SearchType type, bool allow_errors>
std::unique_ptr<TrieIter> Trie::search(const std::string& query) {
    return std::make_unique<TrieIterSearch<type, allow_errors>>(root_node->clone(), query);
}


void Trie::print_trie(std::ostream& os, std::vector<std::string>&& text) {
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


template std::unique_ptr<TrieIter> Trie::search<SearchType::Match , false>(const std::string&);
template std::unique_ptr<TrieIter> Trie::search<SearchType::Match , true >(const std::string&);
template std::unique_ptr<TrieIter> Trie::search<SearchType::Prefix, false>(const std::string&);
template std::unique_ptr<TrieIter> Trie::search<SearchType::Prefix, true >(const std::string&);


} // namespace TextSearch
