#pragma once

#include <string>

#include "graph_models/object_id.h"
#include "storage/index/bplus_tree/bplus_tree.h"
#include "storage/index/text_search/table.h"
#include "storage/index/text_search/tokenize_normalize.h"
#include "storage/index/text_search/trie.h"

namespace TextSearch {

// These are the model specific functions that have to be implemented

typedef void IndexPredicate(Trie&          trie,
                            BPlusTree<2>&  bpt,
                            Table&         table,
                            TokenizeFunc*  tokenize,
                            NormalizeFunc* normalize,
                            std::string    predicate);

typedef std::string OidToString(ObjectId oid);

} // namespace TextSearch
