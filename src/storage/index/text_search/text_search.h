#pragma once

#include <filesystem>
#include <memory>

#include "storage/index/text_search/index_iter.h"
#include "storage/index/text_search/model_funcs.h"
#include "storage/index/text_search/multi_iter.h"
#include "storage/index/text_search/table.h"
#include "storage/index/text_search/text_search_iter.h"
#include "storage/index/text_search/tokenize_normalize.h"
#include "storage/index/text_search/trie.h"

namespace TextSearch {

class TextSearch {
public:
    TextSearch() {};
    TextSearch(std::filesystem::path db_dir,
               size_t                table_column_count,
               TokenizeFunc*         tokenize,
               NormalizeFunc*        normalize,
               IndexPredicate*       index_predicate,
               OidToString*          oid_to_string) :
        db_dir               (db_dir),
        tokenize             (tokenize),
        normalize            (normalize),
        index_predicate_impl (index_predicate),
        oid_to_string        (oid_to_string),
        table_column_count   (table_column_count) { }

    // Checks if an index exists
    bool index_exists(std::string index_name);

    // Creates an index
    void create_index(std::string index_name);

    // Load an index created previously
    bool load_index(std::string index);

    // Index a predicate and add it to the currently loaded index
    void index_predicate(std::string predicate);

    // Return an iterator for the all strings contained in the trie
    TrieIterList get_iter_list();

    std::vector<std::string> process_query(const std::string& query);

    template<SearchType type, bool allow_errors>
    std::unique_ptr<TextSearchIter> search(const std::string& query);

    // Print the index to os in DOT format (Graphviz)
    void print_trie(std::ostream& os, std::vector<std::string>&& text_list) const;

    std::string get_string(uint64_t table_pointer) const;

// private:
    // The directory of the database being used
    std::filesystem::path         db_dir;
    // The path to the directory containing the currently loaded index
    std::filesystem::path         index_dir;
    // The trie of the currently loaded index
    std::unique_ptr<Trie>         trie;
    // The BPT of the currently loaded index
    std::unique_ptr<BPlusTree<2>> bpt;
    // The table of the currently loaded index
    std::unique_ptr<Table>        table;

    // Function used to tokenize the text being indexed
    TokenizeFunc*  tokenize;
    // Function to normalize tokens during indexation, and queries when searching
    NormalizeFunc* normalize;
    // Model specific indexing implementation
    IndexPredicate* index_predicate_impl;
    // Model specific Oid to std::string conversion function
    OidToString* oid_to_string;

    size_t table_column_count;
};

} // namespace TextSearch
