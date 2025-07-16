#pragma once

#include "graph_models/model_destroyer.h"
#include "graph_models/quad_model/quad_catalog.h"
#include "query/parser/paths/regular_path_expr.h"

template <std::size_t N> class BPlusTree;

namespace MQL {
    class OpInsert;
}

class QuadModel {
public:
    QuadCatalog catalog;

    std::unique_ptr<BPlusTree<1>> nodes;

    std::unique_ptr<BPlusTree<2>> node_label;
    std::unique_ptr<BPlusTree<2>> label_node;

    std::unique_ptr<BPlusTree<3>> object_key_value;
    std::unique_ptr<BPlusTree<3>> key_value_object;

    std::unique_ptr<BPlusTree<4>> from_to_type_edge;
    std::unique_ptr<BPlusTree<4>> to_type_from_edge;
    std::unique_ptr<BPlusTree<4>> type_from_to_edge;
    std::unique_ptr<BPlusTree<4>> type_to_from_edge;
    std::unique_ptr<BPlusTree<4>> edge_from_to_type;

    // special cases
    std::unique_ptr<BPlusTree<3>> equal_from_to;      // (from=to,      type, edge)
    std::unique_ptr<BPlusTree<3>> equal_from_type;    // (from=type,    to,   edge)
    std::unique_ptr<BPlusTree<3>> equal_to_type;      // (to=type,      from, edge)
    std::unique_ptr<BPlusTree<2>> equal_from_to_type; // (from=to=type, edge)

    std::unique_ptr<BPlusTree<3>> equal_from_to_inverted;   // (type, from=to,   edge)
    std::unique_ptr<BPlusTree<3>> equal_from_type_inverted; // (to,   from=type, edge)
    std::unique_ptr<BPlusTree<3>> equal_to_type_inverted;   // (from, to=type,   edge)

    uint64_t MAX_LIMIT = UINT64_MAX;

    // Path mode to use
    PathSearchMode path_mode = PathSearchMode::BFS;

    // necessary to be called before first usage
    static std::unique_ptr<ModelDestroyer> init();

private:
    QuadModel();
};

extern QuadModel& quad_model; // global object
