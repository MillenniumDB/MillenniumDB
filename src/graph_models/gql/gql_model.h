#pragma once

#include "graph_models/gql/gql_catalog.h"
#include "graph_models/model_destroyer.h"

template<std::size_t N>
class BPlusTree;

template<std::size_t N>
class RandomAccessTable;

// This is the worst model, only for completeness.
// The idea is in the future to either have a model with a single label per edge
// or another redundant structure with labels inside the edge
//( like {label, from, to, edge} and {label, to, from, edge})
class GQLModel {
public:
    std::unique_ptr<RandomAccessTable<2>> directed_edges;

    std::unique_ptr<RandomAccessTable<2>> undirected_edges;

    // TODO: deleted_nodes?

    std::unique_ptr<BPlusTree<2>> node_label;
    std::unique_ptr<BPlusTree<2>> label_node;

    std::unique_ptr<BPlusTree<2>> edge_label;
    std::unique_ptr<BPlusTree<2>> label_edge;

    std::unique_ptr<BPlusTree<3>> node_key_value;
    std::unique_ptr<BPlusTree<3>> key_value_node;

    std::unique_ptr<BPlusTree<3>> edge_key_value;
    std::unique_ptr<BPlusTree<3>> key_value_edge;

    std::unique_ptr<BPlusTree<3>> from_to_edge;
    std::unique_ptr<BPlusTree<3>> to_from_edge;

    std::unique_ptr<BPlusTree<3>> u_edge;

    std::unique_ptr<BPlusTree<2>> equal_u_edge;

    std::unique_ptr<BPlusTree<2>> equal_d_edge;

    GQLCatalog catalog;

    // necessary to be called before first usage
    static std::unique_ptr<ModelDestroyer> init();

private:
    GQLModel();
};

extern GQLModel& gql_model; // global object
