#include "rdf_model_index_provider.h"

#include "graph_models/rdf_model/rdf_model.h"

using namespace Paths;

std::unique_ptr<EdgeIter> RdfModelIndexProvider::get_btree_iter(uint64_t type_id, bool inverse) {
    Record<3> min_ids = {type_id, 0, 0};
    Record<3> max_ids = {type_id, UINT64_MAX, UINT64_MAX};

    size_t starting_node_index = inverse ? 1 : 2;
    size_t reached_node_index  = inverse ? 2 : 1;
    return std::make_unique<BTreeIndexIterator<3>>(
        rdf_model.pos->get_range(interruption_requested, min_ids, max_ids),
        starting_node_index,
        reached_node_index
    );
}


std::unique_ptr<EdgeIter> RdfModelIndexProvider::get_btree_iter(uint64_t type_id, bool inverse, uint64_t node_id) {

    // Get iter from correct B+Tree
    if (inverse) {
        Record<3> min_ids = {type_id, node_id, 0};
        Record<3> max_ids = {type_id, node_id, UINT64_MAX};
        return std::make_unique<BTreeIndexIterator<3>>(
            rdf_model.pos->get_range(interruption_requested, min_ids, max_ids),
            1,
            2
        );
    } else {
        Record<3> min_ids = {node_id, type_id, 0};
        Record<3> max_ids = {node_id, type_id, UINT64_MAX};
        return std::make_unique<BTreeIndexIterator<3>>(
            rdf_model.spo->get_range(interruption_requested, min_ids, max_ids),
            0,
            2
        );
    }
}


std::unique_ptr<EdgeIter> RdfModelIndexProvider::get_iter(uint64_t type_id, bool inverse) {
    if (inverse) {
        return get_btree_iter(type_id, inverse);
    } else {
        return get_btree_iter(type_id, inverse);
    }
}


std::unique_ptr<EdgeIter> RdfModelIndexProvider::get_iter(uint64_t type_id, bool inverse, uint64_t node_id) {
    if (inverse) {
        // auto assigned_index = t_inv_info.find(type_id);
        // if (assigned_index->second == IndexType::BTREE) {
            return get_btree_iter(type_id, inverse, node_id);
        // }
        // auto index = quad_model.get_index(type_id, inverse, assigned_index->second, interruption_requested);
        // return index->get_iter(node_id, interruption_requested);
    } else {
        // auto assigned_index = t_info.find(type_id);
        // if (assigned_index->second == IndexType::BTREE) {
            return get_btree_iter(type_id, inverse, node_id);
        // }
        // auto index = quad_model.get_index(type_id, inverse, assigned_index->second, interruption_requested);
        // return index->get_iter(node_id, interruption_requested);
    }
}
