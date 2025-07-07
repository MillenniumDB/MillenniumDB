#pragma once

#include <memory>
#include <random>
#include <string>

#include <boost/dynamic_bitset.hpp>
#include <boost/unordered/unordered_map.hpp>

#include "graph_models/common/datatypes/tensor/tensor.h"
#include "graph_models/object_id.h"
#include "storage/index/hnsw/hnsw_entry.h"
#include "storage/index/hnsw/hnsw_entry_flat_map.h"
#include "storage/index/hnsw/hnsw_heap.h"
#include "storage/index/hnsw/hnsw_metric.h"
#include "storage/index/hnsw/hnsw_node.h"
#include "storage/index/hnsw/hnsw_query_iterator.h"

namespace HNSW {
class HNSWIndex {
    friend class HNSWIndexManager;
    friend class HNSWQueryIterator;

public:
    struct HNSWIndexParams {
        uint64_t entry_point_id;  // entry id
        uint64_t dimensions;      // tensor dimensions
        uint64_t layers;          // current number of layers
        uint64_t M;               // max neighbors at construction
        uint64_t ef_construction; // max candidates at construction
    };

    const HNSWIndexParams& get_params() const
    {
        return params;
    };

    static constexpr char DATA_FILENAME[] = "hnsw.dat";

    // create a new index in memory
    // create a new index in memory
    static std::unique_ptr<HNSWIndex> create(
        const std::string& hnsw_index_name,
        uint64_t dimensions,
        uint64_t max_neighbors,
        uint64_t ef_construction,
        MetricFuncType metric_func
    );

    // load an index from disk
    static std::unique_ptr<HNSWIndex> load(const std::string& hnsw_index_name, MetricFuncType metric_func);

    // find the the top num_neighbors from a pool of num_candidates
    HNSWHeap query(
        const tensor::Tensor<float>& query_tensor,
        uint64_t num_neighbors,
        uint64_t num_candidates,
        HNSWVisitedSet* visited_set,
        HNSWHeap* discarded_heap_ptr
    );

    // get an iterator that traverses all nodes in an ANN-manner
    std::unique_ptr<HNSWQueryIterator> query_iterator(
        const bool* interruption_requested,
        tensor::Tensor<float>&& query_tensor,
        uint64_t num_neighbors,
        uint64_t num_candidates
    );

    // resume the search (iterative querying) it is expected that the previous discarded_map will be the current entry points
    HNSWHeap resume_query(
        const tensor::Tensor<float>& query_tensor,
        uint64_t batch_size,
        const std::vector<Entry>& entry_points,
        HNSWVisitedSet* visted_set_ptr,
        HNSWHeap* discarded_heap_ptr
    );

    // index all { subject, object } pairs given a predicate (SPARQL)
    std::tuple<uint_fast32_t> index_predicate(const std::string& predicate);

    // index all { object, value } pairs given a key (MQL)
    std::tuple<uint_fast32_t> index_property(const std::string& key);

    // index a single entry
    template<bool CheckTombstones>
    bool index_single(ObjectId ref_object_id, ObjectId tensor_object_id);

    // mark a single entry as deleted
    bool remove_single(ObjectId ref_object_id, ObjectId tensor_object_id);

    inline const HNSWNode& get_node(uint32_t node_id) const
    {
        assert(node_id < node_storage.size());
        return node_storage[node_id];
    }

    inline std::size_t size() const
    {
        return node_storage.size();
    }

private:
    bool has_changes { false };

    MetricFuncType metric_func;

    HNSWIndexParams params;

    // useful constants
    uint64_t M0;                // max neighbors per node at layer 0
    uint64_t Mi;                // max neighbors per node at layer i > 0
    double random_layer_factor; // normalization factor for random layer selection

    // used for generating levels
    std::random_device device;
    std::default_random_engine level_generator { device() };
    std::uniform_real_distribution<double> distribution { 0.0l, 1.0l };

    // main node storage
    std::vector<HNSWNode> node_storage;

    // node_neighbors_at_layer[i][j] is the map of neighbors for the node i at layer j
    // NOTE: This could be replaced with an array of node_ids, but the set_neighbors_at_layer we
    // would need to calculate the current-worst distance for each node in new_neighbors parameter.
    // This trade-off could be evaluated in the future.
    std::vector<std::vector<HNSWEntryFlatMap>> node_neighbors_at_layer;

    // inverse of node_storage for { subject, object } lookup
    boost::unordered_map<HNSWNode, uint32_t, HNSWNode::Hasher> hnsw_node2node_id;

    // deleted nodes
    boost::dynamic_bitset<> node_tombstones;

    // Create constructor
    explicit HNSWIndex(HNSWIndexParams params, MetricFuncType metric_func);

    // Load constructor
    explicit HNSWIndex(
        HNSWIndexParams params,
        MetricFuncType metric_func,
        std::vector<HNSWNode>&& node_storage,
        std::vector<std::vector<HNSWEntryFlatMap>>&& node_neighbors_at_layer,
        boost::dynamic_bitset<>&& node_tombstones
    );

    // initialize constants for the index
    inline void init_constants()
    {
        M0 = 2 * params.M;
        Mi = params.M;
        random_layer_factor = 1.0l / std::log(static_cast<double>(params.M));
    }

    inline uint64_t create_new_node(ObjectId object_oid, ObjectId tensor_oid, uint64_t top_layer)
    {
        const uint32_t node_id = node_storage.size();
        // create node
        node_storage.emplace_back(object_oid, tensor_oid);

        // create node's neighbors
        auto neighbors = std::vector<HNSWEntryFlatMap>(top_layer + 1);
        node_neighbors_at_layer.emplace_back(std::move(neighbors));

        // create tombstone slot
        node_tombstones.push_back(false);

        // add to inverse lookup
        hnsw_node2node_id.emplace(node_storage.back(), node_id);

        return node_id;
    }

    // search the top num_neighbors at a layer given a query and a group of entry points
    template<bool CheckTombstones>
    HNSWHeap search_at_layer(
        const tensor::Tensor<float>& query_tensor,
        const std::vector<Entry>& entry_points,
        uint64_t num_neighbors,
        uint64_t layer,
        /* Arguments for resuming the iterative querying */
        // previously visited node_ids (iterative querying)
        HNSWVisitedSet* visited_set_ptr,
        // storage for discarded candidates (iterative querying)
        HNSWHeap* discarded_heap_ptr
    );

    // connect the node to the new neighbors (both ways) keeping the max edges limit
    void set_neighbors_at_layer(uint32_t node_id, uint64_t layer, const std::vector<Entry>& new_neighbors);

    // generate a random layer number
    inline uint64_t get_random_layer()
    {
        const double random_layer = -std::log(distribution(level_generator)) * random_layer_factor;
        return static_cast<uint64_t>(random_layer);
    }
};
} // namespace HNSW
