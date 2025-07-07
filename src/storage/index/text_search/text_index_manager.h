#pragma once

#include <shared_mutex>

#include <boost/unordered/unordered_flat_map.hpp>

#include "storage/index/text_search/model_funcs.h"
#include "storage/index/text_search/normalize_funcs.h"
#include "storage/index/text_search/text_index.h"
#include "storage/index/text_search/tokenize_funcs.h"

namespace TextSearch {
class TextIndexManager {
    friend class TextSearch::TextIndex;

public:
    struct TextIndexMetadata {
        TOKENIZE_TYPE tokenization_type;
        NORMALIZE_TYPE normalization_type;
        ObjectId predicate_id; // Needed for projecting the predicate model-agnostic
        std::string predicate;

        friend std::ostream& operator<<(std::ostream& os, const TextIndexMetadata& metadata)
        {
            os << "{\"tokenization\": " << '"' << metadata.tokenization_type << '"';
            os << ", \"normalization\": " << '"' << metadata.normalization_type << '"';
            os << ", \"predicate\": \"" << metadata.predicate << "\"}";
            return os;
        }
    };

    static constexpr char TEXT_SEARCH_DIR[] = "text_search_index";

    // Initialize the text index manager
    void init(
        IndexPredicateFuncType* index_predicate_func_impl_ptr,
        IndexSingleFuncType* index_single_func_impl_ptr,
        RemoveSingleFuncType* remove_single_func_impl_ptr,
        ObjectIdToStringFuncType* objectid_to_string_func_ptr
    );

    void load_text_index(const std::string& name, const TextIndexMetadata& metadata);

    // Returns nullptr if the text index was not found
    TextIndex* get_text_index(const std::string& name);

    // Create a new text index with the given name and predicate. Returns [total_inserted_elements, total_inserted_tokens]
    std::tuple<uint_fast32_t, uint_fast32_t> create_text_search_index(
        const std::string& name,
        const std::string& predicate,
        NORMALIZE_TYPE normalization_type,
        TOKENIZE_TYPE tokenization_type
    );

    boost::unordered_flat_map<std::string, TextIndexMetadata> get_name2metadata() const
    {
        return name2metadata;
    }

    boost::unordered_flat_map<std::string, std::vector<std::string>> get_predicate2names() const
    {
        return predicate2names;
    }

    std::vector<std::string> get_index_names()
    {
        std::shared_lock lck(name2text_search_mutex);

        std::vector<std::string> res;
        res.reserve(name2text_search_index.size());
        for (const auto& [name, _] : name2text_search_index) {
            res.emplace_back(name);
        }

        return res;
    }

    bool has_changes() const
    {
        return has_changes_;
    }

    std::size_t num_text_search_indexes() const
    {
        return name2text_search_index.size();
    }

private:
    bool has_changes_ = false;

    std::tuple<uint_fast32_t, uint_fast32_t, ObjectId> index_predicate(
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

        return (*index_single_func_impl_ptr)(
            trie,
            bpt,
            table,
            tokenize_func,
            normalize_func,
            ref_oid,
            str_oid
        );
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

        return (*remove_single_func_impl_ptr)(
            trie,
            bpt,
            table,
            tokenize_func,
            normalize_func,
            ref_oid,
            str_oid
        );
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

    // Name to text index
    boost::unordered_flat_map<std::string, std::unique_ptr<TextIndex>> name2text_search_index;
    // Name to text index metadata (normalize, tokenize, predicate, etc...)
    boost::unordered_flat_map<std::string, TextIndexMetadata> name2metadata;
    // Predicate to text index name
    boost::unordered_flat_map<std::string, std::vector<std::string>> predicate2names;
};
} // namespace TextSearch
