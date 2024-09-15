#include "index_iter.h"

#include <memory>

#include "graph_models/object_id.h"
#include "storage/index/bplus_tree/bplus_tree.h"
#include "storage/index/text_search/trie_iter.h"
#include "storage/index/text_search/utils.h"


namespace TextSearch {


bool IndexIter::next() {
    if (bpt_iter != nullptr) {
        auto record = bpt_iter->next();
        if (record == nullptr) {
            bpt_iter = nullptr;
        } else {
            auto [node_id, score, table_pointer_] = decompress((*record)[0], (*record)[1]);
            table_pointer = table_pointer_;
            return true;
        }
    }

    if (trie_iter != nullptr) {
        if (trie_iter->next()) {
            auto node_id = trie_iter->get_node_id();

            auto [min_first, min_second] = compress(node_id, 0, 0 );
            auto [max_first, max_second] = compress(node_id, UINT32_MAX, UINT64_MAX );

            Record<2> min({ min_first, min_second });
            Record<2> max({ max_first, max_second });

            bpt_iter = std::make_unique<BptIter<2>>(bpt.get_range(&interruption_requested, min, max));
            return next();
        } else {
            trie_iter = nullptr;
            return false;
        }
    }

    return false;
}


uint64_t IndexIter::get_table_pointer() const {
    return table_pointer;
}


} // namespace TextSearch
