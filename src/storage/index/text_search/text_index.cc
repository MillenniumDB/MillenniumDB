#include "text_index.h"

#include <filesystem>
#include <memory>

#include "storage/index/bplus_tree/bplus_tree.h"
#include "storage/index/text_search/index_iter.h"
#include "storage/index/text_search/multi_iter.h"
#include "storage/index/text_search/text_index_manager.h"
#include "storage/index/text_search/trie.h"
#include "system/file_manager.h"

namespace fs = std::filesystem;

namespace TextSearch {

TextIndex::TextIndex(
    bool create_new,
    const std::string& text_index_name,
    NORMALIZE_TYPE normalize_type,
    TOKENIZE_TYPE tokenize_type,
    TextIndexManager& text_search_index_manager
) :
    text_search_index_manager(text_search_index_manager)
{
    const auto relative_index_path = fs::path(TextIndexManager::TEXT_SEARCH_DIR) / text_index_name;
    if (create_new) {
        const auto absolute_index_path = file_manager.get_file_path(relative_index_path);

        if (!fs::create_directories(absolute_index_path)) {
            throw std::runtime_error("Could not create directories: " + absolute_index_path);
        }
    }

    trie = std::make_unique<Trie>(relative_index_path / TRIE_FILENAME);
    bpt = std::make_unique<BPlusTree<2>>(relative_index_path / BPT_NAME);
    table = std::make_unique<Table>(relative_index_path / TABLE_FILENAME, 2);
    normalize_func = get_normalize_func(normalize_type);
    tokenize_func = get_tokenize_func(tokenize_type);
}

std::tuple<uint_fast32_t, uint_fast32_t, ObjectId> TextIndex::index_predicate(const std::string& predicate)
{
    return text_search_index_manager
        .index_predicate(*trie, *bpt, *table, predicate, normalize_func, tokenize_func);
}

uint_fast32_t TextIndex::index_single(ObjectId ref_oid, ObjectId str_oid)
{
    return text_search_index_manager
        .index_single(*trie, *bpt, *table, ref_oid, str_oid, normalize_func, tokenize_func);
}

uint_fast32_t TextIndex::remove_single(ObjectId ref_oid, ObjectId str_oid)
{
    return text_search_index_manager
        .remove_single(*trie, *bpt, *table, ref_oid, str_oid, normalize_func, tokenize_func);
}

TrieIterList TextIndex::get_iter_list()
{
    assert(trie != nullptr);

    return trie->get_iter_list();
}

std::string TextIndex::get_string(uint64_t table_pointer) const
{
    auto table_row = table->get(table_pointer);

    std::string string;
    for (size_t i = 0; i < table_row.size(); i++) {
        if (i != 0)
            string += ", ";
        string += text_search_index_manager.objectid_to_string(ObjectId(table_row[i]));
    }

    return string;
}

template<SearchType type, bool allow_errors>
std::unique_ptr<TextSearchIter> TextIndex::search(const std::string& query) const
{
    assert(trie != nullptr);

    std::vector<std::unique_ptr<IndexIter>> iters;
    const auto tokenized = tokenize_func(query);
    for (const auto& token : tokenized) {
        const auto normalized = normalize_func(token);
        auto iter = trie->search<type, allow_errors>(normalized);
        iters.push_back(std::make_unique<IndexIter>(std::move(iter), *bpt));
    }

    assert(!iters.empty());

    if (iters.size() > 1) {
        return std::make_unique<MultiIter>(std::move(iters));
    }

    return std::move(iters[0]);
}

void TextIndex::print_trie(std::ostream& os, std::vector<std::string>&& text_list) const
{
    assert(trie != nullptr);

    trie->print_trie(os, std::move(text_list));
}

template std::unique_ptr<TextSearchIter>
    TextIndex::search<SearchType::MATCH, false>(const std::string&) const;
template std::unique_ptr<TextSearchIter> TextIndex::search<SearchType::MATCH, true>(const std::string&) const;
template std::unique_ptr<TextSearchIter>
    TextIndex::search<SearchType::PREFIX, false>(const std::string&) const;
template std::unique_ptr<TextSearchIter>
    TextIndex::search<SearchType::PREFIX, true>(const std::string&) const;

} // namespace TextSearch
