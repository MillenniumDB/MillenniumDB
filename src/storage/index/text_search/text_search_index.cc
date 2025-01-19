#include "text_search_index.h"

#include <filesystem>
#include <memory>

#include "storage/index/bplus_tree/bplus_tree.h"
#include "storage/index/text_search/index_iter.h"
#include "storage/index/text_search/multi_iter.h"
#include "storage/index/text_search/text_search_index_manager.h"
#include "storage/index/text_search/trie.h"
#include "system/file_manager.h"

namespace fs = std::filesystem;

namespace TextSearch {

std::unique_ptr<TextSearchIndex> TextSearchIndex::create(
    const std::string& text_index_name,
    NORMALIZE_TYPE normalize_type,
    TOKENIZE_TYPE tokenize_type,
    TextSearchIndexManager& text_search_index_manager
)
{
    const auto relative_index_path = fs::path(TextSearchIndexManager::TEXT_SEARCH_INDEX_DIR)
                                   / text_index_name;
    const auto absolute_index_path = file_manager.get_file_path(relative_index_path);

    if (!fs::create_directories(absolute_index_path)) {
        throw std::runtime_error("Could not create directories: " + absolute_index_path);
    }

    auto trie = Trie::create(relative_index_path / TRIE_FILENAME);
    auto table = Table::create(relative_index_path / TABLE_FILENAME, 2);

    file_manager.init_file(std::string(relative_index_path / BPT_NAME) + ".dir");
    file_manager.init_file(std::string(relative_index_path / BPT_NAME) + ".leaf");
    auto bpt = std::make_unique<BPlusTree<2>>(relative_index_path / BPT_NAME);

    auto* normalize_func = get_normalize_func(normalize_type);
    auto* tokenize_func = get_tokenize_func(tokenize_type);

    return std::unique_ptr<TextSearchIndex>(new TextSearchIndex(
        std::move(trie),
        std::move(bpt),
        std::move(table),
        normalize_func,
        tokenize_func,
        text_search_index_manager
    ));
}

std::unique_ptr<TextSearchIndex> TextSearchIndex::load(
    const std::string& text_index_name,
    NORMALIZE_TYPE normalize_type,
    TOKENIZE_TYPE tokenize_type,
    TextSearchIndexManager& text_search_index_manager
)
{
    const auto relative_index_path = fs::path(TextSearchIndexManager::TEXT_SEARCH_INDEX_DIR)
                                   / text_index_name;

    // Check that all necessary files exists
    auto trie = Trie::load(relative_index_path / TRIE_FILENAME);
    auto bpt = std::make_unique<BPlusTree<2>>(relative_index_path / BPT_NAME);
    auto table = Table::load(relative_index_path / TABLE_FILENAME);

    auto* normalize_func = get_normalize_func(normalize_type);
    auto* tokenize_func = get_tokenize_func(tokenize_type);

    return std::unique_ptr<TextSearchIndex>(new TextSearchIndex(
        std::move(trie),
        std::move(bpt),
        std::move(table),
        normalize_func,
        tokenize_func,
        text_search_index_manager
    ));
}

TextSearchIndex::TextSearchIndex(
    std::unique_ptr<Trie> trie_,
    std::unique_ptr<BPlusTree<2>> bpt_,
    std::unique_ptr<Table> table_,
    NormalizeFuncType* normalize_func_,
    TokenizeFuncType* tokenize_func_,
    TextSearchIndexManager& text_search_index_manager_
) :
    trie { std::move(trie_) },
    bpt { std::move(bpt_) },
    table { std::move(table_) },
    normalize_func { normalize_func_ },
    tokenize_func { tokenize_func_ },
    text_search_index_manager { text_search_index_manager_ }
{ }

std::tuple<uint_fast32_t, uint_fast32_t> TextSearchIndex::index_predicate(const std::string& predicate)
{
    return text_search_index_manager
        .index_predicate(*trie, *bpt, *table, predicate, normalize_func, tokenize_func);
}

uint_fast32_t TextSearchIndex::index_single(ObjectId ref_oid, ObjectId str_oid)
{
    return text_search_index_manager
        .index_single(*trie, *bpt, *table, ref_oid, str_oid, normalize_func, tokenize_func);
}

uint_fast32_t TextSearchIndex::remove_single(ObjectId ref_oid, ObjectId str_oid)
{
    return text_search_index_manager
        .remove_single(*trie, *bpt, *table, ref_oid, str_oid, normalize_func, tokenize_func);
}

TrieIterList TextSearchIndex::get_iter_list()
{
    assert(trie != nullptr);

    return trie->get_iter_list();
}

std::string TextSearchIndex::get_string(uint64_t table_pointer) const
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
std::unique_ptr<TextSearchIter> TextSearchIndex::search(const std::string& query)
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

void TextSearchIndex::print_trie(std::ostream& os, std::vector<std::string>&& text_list) const
{
    assert(trie != nullptr);

    trie->print_trie(os, std::move(text_list));
}

template std::unique_ptr<TextSearchIter>
    TextSearchIndex::search<SearchType::Match, false>(const std::string&);
template std::unique_ptr<TextSearchIter> TextSearchIndex::search<SearchType::Match, true>(const std::string&);
template std::unique_ptr<TextSearchIter>
    TextSearchIndex::search<SearchType::Prefix, false>(const std::string&);
template std::unique_ptr<TextSearchIter>
    TextSearchIndex::search<SearchType::Prefix, true>(const std::string&);

} // namespace TextSearch
