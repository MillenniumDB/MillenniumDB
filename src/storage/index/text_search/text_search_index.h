#pragma once

#include <memory>

#include "graph_models/object_id.h"
#include "storage/index/bplus_tree/bplus_tree.h"
#include "storage/index/text_search/normalize_funcs.h"
#include "storage/index/text_search/table.h"
#include "storage/index/text_search/text_search_iter.h"
#include "storage/index/text_search/tokenize_funcs.h"
#include "storage/index/text_search/trie.h"

namespace TextSearch {
class TextSearchIndexManager;

class TextSearchIndex {
public:
    static constexpr char TRIE_FILENAME[] = "trie.dat";

    static constexpr char TRIE_GARBAGE_FILENAME[] = "trie.garbage";

    static constexpr char BPT_NAME[] = "bpt";

    static constexpr char TABLE_FILENAME[] = "table.dat";

    // Initialize a new text search index
    static std::unique_ptr<TextSearchIndex> create(
        const std::string& text_index_name,
        NORMALIZE_TYPE normalization_type,
        TOKENIZE_TYPE tokenization_type,
        TextSearchIndexManager& text_search_index_manager
    );

    // Load an existing text search index
    static std::unique_ptr<TextSearchIndex>
        load(const std::string& text_index_name, NORMALIZE_TYPE normalization_type, TOKENIZE_TYPE tokenization_type, TextSearchIndexManager& text_search_index_manager);

    TextSearchIndex(const TextSearchIndex&) = delete;
    TextSearchIndex& operator=(const TextSearchIndex&) = delete;
    // TextSearchIndex(TextSearchIndex&&) = delete;
    // TextSearchIndex& operator=(TextSearchIndex&&) = delete;

    ~TextSearchIndex() = default;

    // Index all strings for the predicate and add it to the currently loaded index
    std::tuple<uint_fast32_t, uint_fast32_t> index_predicate(const std::string& predicate);

    // Index a single string
    uint_fast32_t index_single(ObjectId ref_oid, ObjectId str_oid);

    // Remove an indexed item
    uint_fast32_t remove_single(ObjectId ref_oid, ObjectId str_oid);

    // Return an iterator for the all strings contained in the trie
    TrieIterList get_iter_list();

    template<SearchType type, bool allow_errors>
    std::unique_ptr<TextSearchIter> search(const std::string& query);

    // private:
    explicit TextSearchIndex(
        std::unique_ptr<Trie> trie,
        std::unique_ptr<BPlusTree<2>> bpt,
        std::unique_ptr<Table> table,
        NormalizeFuncType* normalize_func,
        TokenizeFuncType* tokenize_func,
        TextSearchIndexManager& text_search_index_manager
    );

    std::string get_string(uint64_t table_pointer) const;

    // Print the index to os in DOT format (Graphviz)
    void print_trie(std::ostream& os, std::vector<std::string>&& text_list) const;

    // The trie of the currently loaded index
    std::unique_ptr<Trie> trie;
    // The BPT of the currently loaded index
    std::unique_ptr<BPlusTree<2>> bpt;
    // The table of the currently loaded index
    std::unique_ptr<Table> table;

    NormalizeFuncType* normalize_func;
    TokenizeFuncType* tokenize_func;

    TextSearchIndexManager& text_search_index_manager;
};

} // namespace TextSearch
