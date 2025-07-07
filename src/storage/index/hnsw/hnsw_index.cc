#include "hnsw_index.h"

#include <filesystem>
#include <memory>

#include "graph_models/quad_model/quad_model.h"
#include "graph_models/quad_model/quad_object_id.h"
#include "graph_models/rdf_model/conversions.h"
#include "graph_models/rdf_model/rdf_model.h"
#include "query/optimizer/quad_model/plan/property_plan.h"
#include "query/optimizer/rdf_model/plan/triple_plan.h"
#include "query/query_context.h"
#include "storage/index/hnsw/hnsw_heap.h"
#include "storage/index/hnsw/hnsw_index_manager.h"
#include "storage/index/hnsw/hnsw_visited_set.h"
#include "system/file_manager.h"

namespace fs = std::filesystem;

namespace HNSW {
std::unique_ptr<HNSWIndex> HNSWIndex::create(
    const std::string& hnsw_index_name,
    uint64_t dimensions,
    uint64_t max_neighbors,
    uint64_t n_candidates_insertion,
    MetricFuncType metric_func
)
{
    const auto relative_index_path = fs::path(HNSWIndexManager::HNSW_INDEX_DIR) / hnsw_index_name;
    const auto absolute_index_path = file_manager.get_file_path(relative_index_path);

    if (!fs::create_directories(absolute_index_path)) {
        throw std::runtime_error("Could not create directories: " + absolute_index_path);
    };

    HNSWIndexParams params {};
    params.entry_point_id = 0;
    params.dimensions = dimensions;
    params.layers = 1;
    params.M = max_neighbors;
    params.ef_construction = n_candidates_insertion;

    return std::unique_ptr<HNSWIndex>(new HNSWIndex(params, metric_func));
}

std::unique_ptr<HNSWIndex> HNSWIndex::load(const std::string& hnsw_index_name, MetricFuncType metric_func)
{
    const auto relative_index_path = fs::path(HNSWIndexManager::HNSW_INDEX_DIR) / hnsw_index_name;
    const auto absolute_index_path = file_manager.get_file_path(relative_index_path);
    const auto absolute_data_file_path = fs::path(absolute_index_path) / HNSWIndex::DATA_FILENAME;

    std::fstream ifs(absolute_data_file_path, std::ios::in | std::ios::binary);

    // load params
    HNSWIndex::HNSWIndexParams params {};
    ifs.read(reinterpret_cast<char*>(&params), sizeof(HNSWIndex::HNSWIndexParams));
    if (!ifs.good()) {
        throw std::runtime_error("Could not read index file");
    }

    uint64_t num_nodes;
    ifs.read(reinterpret_cast<char*>(&num_nodes), sizeof(uint64_t));
    if (!ifs.good()) {
        throw std::runtime_error("Could not read index file");
    }

    std::vector<HNSWNode> node_storage;
    std::vector<std::vector<HNSWEntryFlatMap>> node_neighbors;

    node_storage.reserve(num_nodes);
    node_neighbors.reserve(num_nodes);

    for (uint32_t node_id = 0; node_id < num_nodes; ++node_id) {
        // read node
        HNSWNode node {};
        ifs.read(reinterpret_cast<char*>(&node), sizeof(HNSWNode));
        if (!ifs.good()) {
            throw std::runtime_error("Could not read index file");
        }
        node_storage.emplace_back(std::move(node));

        // read neighbors
        uint64_t num_layers;
        ifs.read(reinterpret_cast<char*>(&num_layers), sizeof(uint64_t));
        if (!ifs.good()) {
            throw std::runtime_error("Could not read index file");
        }

        node_neighbors.emplace_back(std::vector<HNSWEntryFlatMap>(num_layers));

        for (uint64_t layer_i = 0; layer_i < num_layers; ++layer_i) {
            uint64_t num_neighbors_at_layer_i;
            ifs.read(reinterpret_cast<char*>(&num_neighbors_at_layer_i), sizeof(uint64_t));
            if (!ifs.good()) {
                throw std::runtime_error("Could not read index file");
            }

            Entry entry_buf;
            for (uint64_t neighbor_i = 0; neighbor_i < num_neighbors_at_layer_i; ++neighbor_i) {
                ifs.read(reinterpret_cast<char*>(&entry_buf), sizeof(entry_buf));
                if (!ifs.good()) {
                    throw std::runtime_error("Could not read index file");
                }
                node_neighbors[node_id][layer_i].emplace(entry_buf);
            }
        }
    }

    // read tombstones
    using block_type = boost::dynamic_bitset<>::block_type;
    boost::dynamic_bitset<> tombstone_bitset(node_storage.size());

    std::vector<block_type> tombstone_blocks(tombstone_bitset.num_blocks());
    ifs.read(reinterpret_cast<char*>(tombstone_blocks.data()), sizeof(block_type) * tombstone_blocks.size());
    if (!ifs.good()) {
        throw std::runtime_error("Could not read index file");
    }
    boost::from_block_range(tombstone_blocks.begin(), tombstone_blocks.end(), tombstone_bitset);

    return std::unique_ptr<HNSWIndex>(new HNSWIndex(
        params,
        metric_func,
        std::move(node_storage),
        std::move(node_neighbors),
        std::move(tombstone_bitset)
    ));
}

HNSWHeap HNSWIndex::query(
    const tensor::Tensor<float>& query_tensor,
    uint64_t num_neighbors,
    uint64_t num_candidates,
    HNSWVisitedSet* visited_set,
    HNSWHeap* discarded_heap_ptr
)
{
    const uint64_t top_layer = params.layers - 1;

    // Initialize with entry point
    const auto entry_tensor = Common::Conversions::to_tensor<float>(
        node_storage[params.entry_point_id].tensor_oid
    );
    const float entry_dist = metric_func(query_tensor, entry_tensor);
    std::vector<Entry> entry_points;
    entry_points.emplace_back(entry_dist, params.entry_point_id);

    // find the best set entry points for layer 0
    for (uint64_t current_layer = top_layer; current_layer > 0; --current_layer) {
        const auto current_layer_nns = search_at_layer<true>(
            query_tensor,
            entry_points,
            1,
            current_layer,
            nullptr,
            nullptr
        );
        assert(current_layer_nns.size() == 1);
        entry_points[0] = current_layer_nns.get_min();
    }

    auto nearest_neighbors = search_at_layer<true>(
        query_tensor,
        entry_points,
        num_candidates,
        0,
        visited_set,
        discarded_heap_ptr
    );

    if (discarded_heap_ptr) {
        // modify inplace and store discarded
        while (nearest_neighbors.size() > num_neighbors) {
            discarded_heap_ptr->push(nearest_neighbors.get_max());
            nearest_neighbors.pop_max();
        }
        return nearest_neighbors;
    }

    const bool is_inplace_cheaper = nearest_neighbors.size() < 2 * num_neighbors;
    if (is_inplace_cheaper) {
        // modify inplace
        while (nearest_neighbors.size() > num_neighbors) {
            nearest_neighbors.pop_max();
        }
        return nearest_neighbors;
    }

    // create a new heap
    HNSWHeap res(num_neighbors);
    while (res.size() < num_neighbors) {
        res.push(nearest_neighbors.get_min());
        nearest_neighbors.pop_min();
    }
    return res;
}

std::unique_ptr<HNSWQueryIterator> HNSWIndex::query_iterator(
    const bool* interruption_requested,
    tensor::Tensor<float>&& query_tensor,
    uint64_t num_neighbors,
    uint64_t num_candidates
)
{
    return std::make_unique<HNSWQueryIterator>(
        interruption_requested,
        this,
        std::move(query_tensor),
        num_neighbors,
        num_candidates
    );
}

HNSWHeap HNSWIndex::resume_query(
    const tensor::Tensor<float>& query_tensor,
    uint64_t batch_size,
    const std::vector<Entry>& entry_points,
    HNSWVisitedSet* visited_set_ptr,
    HNSWHeap* discarded_heap_ptr
)
{
    return search_at_layer<true>(
        query_tensor,
        entry_points,
        batch_size,
        0,
        visited_set_ptr,
        discarded_heap_ptr
    );
}

std::tuple<uint_fast32_t> HNSWIndex::index_predicate(const std::string& predicate)
{
    const auto subject_var = get_query_ctx().get_internal_var();
    const auto predicate_val = SPARQL::Conversions::pack_iri(predicate);
    const auto object_var = get_query_ctx().get_internal_var();

    const auto triple_plan = SPARQL::TriplePlan(subject_var, predicate_val, object_var);
    auto triple_plan_iter = triple_plan.get_binding_iter();

    Binding binding(get_query_ctx().get_var_size());
    triple_plan_iter->begin(binding);

    const std::size_t num_expected_insertions = rdf_model.catalog.get_predicate_count(predicate_val.id);

    node_storage.reserve(num_expected_insertions);
    node_neighbors_at_layer.reserve(num_expected_insertions);

    uint_fast32_t total_inserted_elements { 0 };
    while (triple_plan_iter->next()) {
        const auto object_oid = binding[object_var];
        const auto subject_oid = binding[subject_var];

        if (index_single<false>(subject_oid, object_oid)) {
            ++total_inserted_elements;
        }
    }

    return { total_inserted_elements };
}

std::tuple<uint_fast32_t> HNSWIndex::index_property(const std::string& key)
{
    const auto object_var = get_query_ctx().get_internal_var();
    const auto key_val = QuadObjectId::get_string(key);
    const auto value_var = get_query_ctx().get_internal_var();

    const auto property_plan = PropertyPlan(object_var, key_val, value_var);

    auto property_plan_iter = property_plan.get_binding_iter();

    Binding binding(get_query_ctx().get_var_size());
    property_plan_iter->begin(binding);

    auto it = quad_model.catalog.key2total_count.find(key_val.id);
    std::size_t num_expected_insertions = 0;
    if (it != quad_model.catalog.key2total_count.end()) {
        num_expected_insertions = it->second;
    }

    node_storage.reserve(num_expected_insertions);
    node_neighbors_at_layer.reserve(num_expected_insertions);

    uint_fast32_t total_inserted_elements { 0 };
    while (property_plan_iter->next()) {
        const auto object_oid = binding[object_var];
        const auto value_oid = binding[value_var];

        if (index_single<false>(object_oid, value_oid)) {
            ++total_inserted_elements;
        }
    }

    return { total_inserted_elements };
}

template<bool CheckTombstones>
bool HNSWIndex::index_single(ObjectId ref_object_id, ObjectId tensor_object_id)
{
    const auto gen_t = RDF_OID::get_generic_type(tensor_object_id);
    if (gen_t != RDF_OID::GenericType::TENSOR) {
        // Object is not a tensor
        return false;
    }

    if constexpr (CheckTombstones) {
        HNSWNode hnsw_node(ref_object_id, tensor_object_id);

        const auto it = hnsw_node2node_id.find(hnsw_node);
        if (it != hnsw_node2node_id.end()) {
            // entry is already in graph, unset its tombstone
            assert(node_tombstones.test(it->second) && "inserted an already set triple. Something went wrong");
            node_tombstones.set(it->second, false);
            has_changes = true;
            return true;
        }
    }

    const auto query_tensor = Common::Conversions::to_tensor<float>(tensor_object_id);
    if (query_tensor.size() != params.dimensions) {
        // Tensor dimension does not match
        return false;
    }

    // Create new node at a random layer
    const uint64_t node_top_layer = get_random_layer();
    const uint64_t node_id = create_new_node(ref_object_id, tensor_object_id, node_top_layer);

    // Initialize with entry point
    const auto entry_tensor = Common::Conversions::to_tensor<float>(
        node_storage[params.entry_point_id].tensor_oid
    );
    const auto entry_dist = metric_func(query_tensor, entry_tensor);
    std::vector<Entry> entry_points;
    entry_points.emplace_back(entry_dist, params.entry_point_id);

    // Update the best entry point for layers in range [top_layer, node_top_layer)
    const uint64_t top_layer = params.layers - 1;
    for (uint64_t current_layer = top_layer; current_layer > node_top_layer; --current_layer) {
        const auto current_layer_nns = search_at_layer<CheckTombstones>(
            query_tensor,
            entry_points,
            1,
            current_layer,
            nullptr,
            nullptr
        );
        assert(current_layer_nns.size() == 1);
        entry_points[0] = current_layer_nns.get_min();
    }

    // Insert in node's layer and the layers below
    for (int64_t current_layer = std::min(node_top_layer, top_layer); current_layer >= 0; --current_layer) {
        auto current_layer_nns = search_at_layer<CheckTombstones>(
            query_tensor,
            entry_points,
            params.ef_construction,
            current_layer,
            nullptr,
            nullptr
        );
        const uint64_t max_neighbors = current_layer == 0 ? M0 : Mi;

        auto current_layer_top_k = current_layer_nns.extract_n_min(max_neighbors);

        // Mutually connect node with its new neighbors
        set_neighbors_at_layer(node_id, current_layer, current_layer_top_k);

        // Update entry points
        entry_points = std::move(current_layer_top_k);
    }

    if (node_top_layer > top_layer) {
        params.entry_point_id = node_id;
        params.layers = node_top_layer + 1;
    };

    has_changes = true;

    return true;
}

bool HNSWIndex::remove_single(ObjectId ref_object_id, ObjectId tensor_object_id)
{
    HNSWNode hnsw_node(ref_object_id, tensor_object_id);

    const auto it = hnsw_node2node_id.find(hnsw_node);
    assert(it != hnsw_node2node_id.end());

    node_tombstones.set(it->second, true);

    has_changes = true;

    return true;
}

HNSWIndex::HNSWIndex(HNSWIndexParams params_, MetricFuncType metric_func_) :
    metric_func { metric_func_ },
    params { params_ }
{
    init_constants();
}

HNSWIndex::HNSWIndex(
    HNSWIndexParams params_,
    MetricFuncType metric_func_,
    std::vector<HNSWNode>&& node_storage_,
    std::vector<std::vector<HNSWEntryFlatMap>>&& node_neighbors_at_layer_,
    boost::dynamic_bitset<>&& node_tombstones_
) :
    metric_func { metric_func_ },
    params { params_ },
    node_storage { std::move(node_storage_) },
    node_neighbors_at_layer { std::move(node_neighbors_at_layer_) },
    node_tombstones { node_tombstones_ }
{
    init_constants();

    for (std::size_t i = 0; i < node_storage.size(); ++i) {
        hnsw_node2node_id.emplace(node_storage[i], i);
    }
}

template<bool CheckTombstones>
HNSWHeap HNSWIndex::search_at_layer(
    const tensor::Tensor<float>& query_tensor,
    const std::vector<Entry>& entry_points,
    uint64_t num_neighbors,
    uint64_t layer,
    HNSWVisitedSet* visited_set_ptr,
    HNSWHeap* discarded_heap_ptr
)
{
    assert(!entry_points.empty());
    assert(num_neighbors > 0);
    assert(entry_points.size() <= num_neighbors);

    HNSWVisitedSet visited_set_;
    HNSWHeap candidates_heap(num_neighbors);
    HNSWHeap top_k_heap(num_neighbors);

    if (visited_set_ptr != nullptr) {
        for (std::size_t i = 0; i < entry_points.size(); ++i) {
            candidates_heap.emplace(entry_points[i]);
            top_k_heap.emplace(entry_points[i]);
            visited_set_ptr->emplace(entry_points[i].node_id);
        }
    } else {
        visited_set_.reserve(node_storage.size());
        // no visited set was provided, initialize a new one
        for (std::size_t i = 0; i < entry_points.size(); ++i) {
            visited_set_.emplace(entry_points[i].node_id);
            candidates_heap.emplace(entry_points[i]);
            top_k_heap.emplace(entry_points[i]);
        }

        visited_set_ptr = &visited_set_;
    }

    while (!candidates_heap.empty()) {
        // get best candidate and worst result
        const Entry candidate_entry = candidates_heap.get_min();
        if (candidate_entry.distance > top_k_heap.get_max().distance) {
            // no further result improvement
            break;
        }

        candidates_heap.pop_min();

        // explore candidate's neighborhood
        for (const Entry& candidate_neighbor_entry : node_neighbors_at_layer[candidate_entry.node_id][layer])
        {
            if (visited_set_ptr->contains(candidate_neighbor_entry.node_id)) {
                continue;
            }
            visited_set_ptr->emplace(candidate_neighbor_entry.node_id);

            const auto candidate_neighbor_tensor = Common::Conversions::to_tensor<float>(
                node_storage[candidate_neighbor_entry.node_id].tensor_oid
            );
            const float candidate_neighbor_dist = metric_func(query_tensor, candidate_neighbor_tensor);

            if constexpr (CheckTombstones) {
                if (node_tombstones.test(candidate_neighbor_entry.node_id)) {
                    // candidate_neighbor_entry.node_id is marked as deleted,
                    // it can be explored but not added to the top_k_heap
                    candidates_heap.emplace(candidate_neighbor_dist, candidate_neighbor_entry.node_id);
                    continue;
                }
            }

            if (top_k_heap.size() < num_neighbors) {
                // always add, not enough results
                candidates_heap.emplace(candidate_neighbor_dist, candidate_neighbor_entry.node_id);
                top_k_heap.emplace(candidate_neighbor_dist, candidate_neighbor_entry.node_id);
                continue;
            }

            if (candidate_neighbor_dist < top_k_heap.get_max().distance) {
                // the candidate improves the result, furthest result must be removed
                if (discarded_heap_ptr != nullptr) {
                    discarded_heap_ptr->emplace(top_k_heap.get_max());
                }
                candidates_heap.emplace(candidate_neighbor_dist, candidate_neighbor_entry.node_id);
                top_k_heap.pop_max();
                top_k_heap.emplace(candidate_neighbor_dist, candidate_neighbor_entry.node_id);
                continue;
            }

            if (discarded_heap_ptr != nullptr) {
                // the candidate does not improve the result
                discarded_heap_ptr->emplace(candidate_neighbor_dist, candidate_neighbor_entry.node_id);
            }
        }
    }

    return top_k_heap;
}

void HNSWIndex::set_neighbors_at_layer(
    uint32_t node_id,
    uint64_t layer,
    const std::vector<Entry>& new_neighbors
)
{
    const auto max_neighbors = layer == 0 ? M0 : Mi;
    assert(new_neighbors.size() <= max_neighbors);

    node_neighbors_at_layer[node_id][layer].reserve(new_neighbors.size());

    for (const Entry& new_neighbor_entry : new_neighbors) {
        // node -> neighbor
        node_neighbors_at_layer[node_id][layer].emplace(new_neighbor_entry);

        // neighbor -> node
        auto& new_neighbor_neighbors_at_layer = node_neighbors_at_layer[new_neighbor_entry.node_id][layer];
        if (new_neighbor_neighbors_at_layer.size() < max_neighbors) {
            // max_neighbors not reached, just add the connection
            new_neighbor_neighbors_at_layer.emplace(new_neighbor_entry.distance, node_id);
        } else {
            // replace if the connection is better than the furthest one
            const auto furthest_it = --new_neighbor_neighbors_at_layer.end();
            if (new_neighbor_entry.distance < furthest_it->distance) {
                new_neighbor_neighbors_at_layer.erase(furthest_it);
                new_neighbor_neighbors_at_layer.emplace(new_neighbor_entry.distance, node_id);
            }
        }
    }
}

} // namespace HNSW

template bool HNSW::HNSWIndex::index_single<false>(ObjectId, ObjectId);
template bool HNSW::HNSWIndex::index_single<true>(ObjectId, ObjectId);
