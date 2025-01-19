#pragma once

#include <string>

#include "graph_models/object_id.h"
#include "storage/index/bplus_tree/bplus_tree.h"
#include "storage/index/text_search/normalize_funcs.h"
#include "storage/index/text_search/table.h"
#include "storage/index/text_search/tokenize_funcs.h"
#include "storage/index/text_search/trie.h"

namespace TextSearch {

// These are the model specific functions that have to be implemented

// Returns [total_inserted_elements, total_inserted_tokens]
typedef std::tuple<uint_fast32_t, uint_fast32_t> IndexPredicateFuncType(
    Trie& trie,
    BPlusTree<2>& bpt,
    Table& table,
    TokenizeFuncType* tokenize,
    NormalizeFuncType* normalize,
    const std::string& predicate
);

// Returns inserted_tokens
typedef uint_fast32_t IndexSingleFuncType(
    Trie& trie,
    BPlusTree<2>& bpt,
    Table& table,
    TokenizeFuncType* tokenize,
    NormalizeFuncType* normalize,
    ObjectId ref_oid,
    ObjectId str_oid
);

// Returns removed_tokens
typedef uint_fast32_t RemoveSingleFuncType(
    Trie& trie,
    BPlusTree<2>& bpt,
    Table& table,
    TokenizeFuncType* tokenize,
    NormalizeFuncType* normalize,
    ObjectId ref_oid,
    ObjectId str_oid
);

typedef std::string ObjectIdToStringFuncType(ObjectId oid);

} // namespace TextSearch
