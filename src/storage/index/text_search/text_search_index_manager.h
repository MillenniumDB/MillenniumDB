#pragma once

#include <shared_mutex>

#include <boost/unordered/unordered_map.hpp>

#include "storage/index/text_search/model_funcs.h"
#include "storage/index/text_search/normalize_funcs.h"
#include "storage/index/text_search/text_search_index.h"
#include "storage/index/text_search/tokenize_funcs.h"

namespace TextSearch {
class TextSearchIndex;
class TextSearchIndexManager {
    friend class TextSearch::TextSearchIndex;

public:
    struct TextSearchIndexMetadata {
        TOKENIZE_TYPE tokenization_type;
        NORMALIZE_TYPE normalization_type;
        std::string predicate;

        friend std::ostream& operator<<(std::ostream& os, const TextSearchIndexMetadata& metadata)
        {
            os << "tokenization : " << metadata.tokenization_type << '\n';
            os << "normalization: " << metadata.normalization_type << '\n';
            os << "predicate    : " << metadata.predicate;
            return os;
        }
    };

    static constexpr char TEXT_SEARCH_INDEX_DIR[] = "text_search_index";

    // Initialize the text search index manager
    void init(
        IndexPredicateFuncType* index_predicate_func_impl_ptr,
        IndexSingleFuncType* index_single_func_impl_ptr,
        RemoveSingleFuncType* remove_single_func_impl_ptr,
        ObjectIdToStringFuncType* objectid_to_string_func_ptr
    );

    void load_text_search_index(const std::string& name, const TextSearchIndexMetadata& metadata);

    // Returns true if the text search was found
    bool get_text_search_index(const std::string& name, TextSearchIndex** text_search_index);

    // Create a new text search index with the given name and predicate. Returns [total_inserted_elements, total_inserted_tokens]
    std::tuple<uint_fast32_t, uint_fast32_t> create_text_search_index(
        const std::string& name,
        const std::string& predicate,
        NORMALIZE_TYPE normalization_type,
        TOKENIZE_TYPE tokenization_type
    );

    const boost::unordered_map<std::string, TextSearchIndexMetadata>& get_name2metadata() const
    {
        return name2metadata;
    }

    const boost::unordered_map<std::string, std::vector<std::string>>& get_predicate2names() const
    {
        return predicate2names;
    }

    bool has_changes() const
    {
        return has_changes_;
    }

private:
    bool has_changes_ = false;

    std::tuple<uint_fast32_t, uint_fast32_t> index_predicate(
        TextSearch::Trie& trie,
        BPlusTree<2>& bpt,
        TextSearch::Table& table,
        const std::string& predicate,
        NormalizeFuncType* normalize_func,
        TokenizeFuncType* tokenize_func
    )
    {
        assert(index_predicate_func_impl_ptr != nullptr);

        return (*index_predicate_func_impl_ptr)(trie, bpt, table, tokenize_func, normalize_func, predicate);
    }

    uint_fast32_t index_single(
        TextSearch::Trie& trie,
        BPlusTree<2>& bpt,
        TextSearch::Table& table,
        ObjectId ref_oid,
        ObjectId str_oid,
        NormalizeFuncType* normalize_func,
        TokenizeFuncType* tokenize_func
    )
    {
        assert(index_predicate_func_impl_ptr != nullptr);

        return (*index_single_func_impl_ptr
        )(trie, bpt, table, tokenize_func, normalize_func, ref_oid, str_oid);
    }

    uint_fast32_t remove_single(
        TextSearch::Trie& trie,
        BPlusTree<2>& bpt,
        TextSearch::Table& table,
        ObjectId ref_oid,
        ObjectId str_oid,
        NormalizeFuncType* normalize_func,
        TokenizeFuncType* tokenize_func
    )
    {
        assert(remove_single_func_impl_ptr != nullptr);

        return (*remove_single_func_impl_ptr
        )(trie, bpt, table, tokenize_func, normalize_func, ref_oid, str_oid);
    }

    std::string objectid_to_string(ObjectId oid) const
    {
        assert(objectid_to_string_func_ptr != nullptr);

        return (*objectid_to_string_func_ptr)(oid);
    }

    // Model specific predicate indexing implementation
    IndexPredicateFuncType* index_predicate_func_impl_ptr;

    // Model specific single indexing implementation
    IndexSingleFuncType* index_single_func_impl_ptr;

    // Model specific remove single implementation
    RemoveSingleFuncType* remove_single_func_impl_ptr;

    // Model specific ObjectId to std::string conversion function
    ObjectIdToStringFuncType* objectid_to_string_func_ptr;

    // Prevents concurrent access to name2text_search
    std::shared_mutex name2text_search_mutex;

    // Name to text search index
    boost::unordered_map<std::string, std::unique_ptr<TextSearchIndex>> name2text_search_index;
    // Predicate to text search index metadata (normalize, tokenize, predicate, etc...)
    boost::unordered_map<std::string, TextSearchIndexMetadata> name2metadata;
    // Predicate to text search index name
    boost::unordered_map<std::string, std::vector<std::string>> predicate2names;
};
} // namespace TextSearch