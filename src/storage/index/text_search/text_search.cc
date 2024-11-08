#include "text_search.h"

#include <filesystem>
#include <memory>

#include "storage/index/bplus_tree/bplus_tree.h"
#include "storage/index/text_search/index_iter.h"
#include "storage/index/text_search/trie.h"


namespace TextSearch {


bool TextSearch::load_index(std::string index) {
    index_dir = "tsi_" + index;

    if (!std::filesystem::is_directory(db_dir / index_dir)) {
        index_dir = "";
        return false;
    }

    trie  = std::make_unique<Trie>        (index_dir / "trie");
    bpt   = std::make_unique<BPlusTree<2>>(index_dir / "bpt");
    table = std::make_unique<Table>       (index_dir / "table", table_column_count);
    return true;
}


bool TextSearch::index_exists(std::string index_name) {
    index_dir = db_dir / ("tsi_" + index_name);

    if (std::filesystem::is_directory(index_dir)) {
        return true;
    } else {
        return false;
    }
}


void TextSearch::create_index(std::string index_name) {
    index_dir = db_dir / ("tsi_" + index_name);

    if (std::filesystem::exists(index_dir)) {
        if (std::filesystem::is_directory(index_dir)) {
            throw std::runtime_error("Index \"" + index_name+ "\" already exists.");
        } else {
            throw std::runtime_error("Index directory path \"" + index_dir.string()+ "\" is a file.");
        }
    }

    if (!std::filesystem::create_directory(index_dir)) {
        throw std::runtime_error("Could not create index directory \"" + index_dir.string() + "\"");
    }
}


void TextSearch::index_predicate(std::string predicate) {
    index_predicate_impl(*trie, *bpt, *table, tokenize, normalize, predicate);
}


TrieIterList TextSearch::get_iter_list() {
    if (trie == nullptr) {
        throw std::logic_error("No index loaded");
    }
    return trie->get_iter_list();
}


std::string TextSearch::get_string(uint64_t table_pointer) const {
    auto table_row = table->get(table_pointer);

    std::string string;
    for (size_t i = 0; i < table_row.size(); i++) {
        if (i != 0) string += ", ";
        string += oid_to_string(ObjectId(table_row[i]));
    }

    return string;
}


template<SearchType type, bool allow_errors>
std::unique_ptr<TextSearchIter> TextSearch::search(const std::string& query) {
     if (trie == nullptr) {
        throw std::logic_error("No index loaded");
    }
    std::vector<std::unique_ptr<IndexIter>> iters;
    auto tokenized = tokenize(query);
    for (auto& token : tokenized) {
        auto normalized = normalize(token);
        auto iter = trie->search<type, allow_errors>(normalized);
        iters.push_back(std::make_unique<IndexIter>(std::move(iter), *bpt));
    }

    if (iters.size() == 1) {
        return std::move(iters[0]);
    } else {
        return std::make_unique<MultiIter>(std::move(iters));
    }
}


void TextSearch::print_trie(std::ostream& os, std::vector<std::string>&& text_list) const {
    if (trie == nullptr) {
        throw std::logic_error("No index loaded");
    }
    trie->print_trie(os, std::move(text_list));
}


template std::unique_ptr<TextSearchIter> TextSearch::search<SearchType::Match , false>(const std::string&);
template std::unique_ptr<TextSearchIter> TextSearch::search<SearchType::Match , true >(const std::string&);
template std::unique_ptr<TextSearchIter> TextSearch::search<SearchType::Prefix, false>(const std::string&);
template std::unique_ptr<TextSearchIter> TextSearch::search<SearchType::Prefix, true >(const std::string&);


} // namespace TextSearch
