#include "hnsw_index_manager.h"

#include <filesystem>
#include <mutex>

#include "misc/logger.h"
#include "system/file_manager.h"

using namespace HNSW;
namespace fs = std::filesystem;

void HNSWIndexManager::init()
{
    const auto hnsw_indexes_path = file_manager.get_file_path(HNSW_INDEX_DIR);
    if (!fs::is_directory(hnsw_indexes_path)) {
        // Hnsw indexes directory did not exist
        fs::create_directories(hnsw_indexes_path);
    }
}

HNSWIndexManager::~HNSWIndexManager()
{
    for (const auto& [name, index] : name2hnsw_index) {
        if (index->has_changes) {
            write_to_disk(name, index.get());
        }
    }
}

void HNSWIndexManager::load_hnsw_index(const std::string& name, const HNSWIndexMetadata& metadata)
{
    try {
        const auto metric_func = metric_type2metric_func(metadata.metric_type);
        auto hnsw_index = HNSWIndex::load(name, metric_func);

        name2hnsw_index[name] = std::move(hnsw_index);
        name2metadata[name] = metadata;
        predicate2names[metadata.predicate].emplace_back(name);
    } catch (const std::exception& e) {
        logger(Category::Error) << "Failed to load HnswIndex \"" + name + "\": " + e.what();
    } catch (...) {
        logger(Category::Error) << "Failed to load HnswIndex \"" + name + "\": Unknown error";
    }
}

void HNSWIndexManager::write_to_disk(const std::string& name, HNSWIndex* index)
{
    const auto relative_index_path = fs::path(HNSWIndexManager::HNSW_INDEX_DIR) / name;
    const auto absolute_index_path = file_manager.get_file_path(relative_index_path);
    const auto absolute_data_file_path = fs::path(absolute_index_path) / HNSWIndex::DATA_FILENAME;

    std::fstream ofs(absolute_data_file_path, std::ios::out | std::ios::trunc | std::ios::binary);
    // write params
    ofs.write(reinterpret_cast<const char*>(&(index->params)), sizeof(HNSWIndex::HNSWIndexParams));
    if (!ofs.good()) {
        throw std::runtime_error("Could not write index file");
    }

    const uint64_t num_nodes = index->node_storage.size();
    ofs.write(reinterpret_cast<const char*>(&num_nodes), sizeof(uint64_t));
    if (!ofs.good()) {
        throw std::runtime_error("Could not write index file");
    }

    for (uint32_t node_id = 0; node_id < num_nodes; ++node_id) {
        // write node
        const HNSWNode& node = index->node_storage[node_id];
        ofs.write(reinterpret_cast<const char*>(&node), sizeof(HNSWNode));
        if (!ofs.good()) {
            throw std::runtime_error("Could not write index file");
        }

        // write neighbors
        const auto& node_neighbors_at_layers = index->node_neighbors_at_layer[node_id];
        const uint64_t num_layers = node_neighbors_at_layers.size();
        ofs.write(reinterpret_cast<const char*>(&num_layers), sizeof(uint64_t));
        if (!ofs.good()) {
            throw std::runtime_error("Could not write index file");
        }

        for (const auto& neighbors_at_layer_i : node_neighbors_at_layers) {
            // layer i size
            const uint64_t num_neighbors_at_layer_i = neighbors_at_layer_i.size();
            ofs.write(reinterpret_cast<const char*>(&num_neighbors_at_layer_i), sizeof(uint64_t));
            if (!ofs.good()) {
                throw std::runtime_error("Could not write index file");
            }

            // layer i data
            for (const auto& neighbor_entry : neighbors_at_layer_i) {
                ofs.write(reinterpret_cast<const char*>(&neighbor_entry), sizeof(neighbor_entry));
                if (!ofs.good()) {
                    throw std::runtime_error("Could not write index file");
                }
            }
        }
    }

    // write tombstones
    using block_type = boost::dynamic_bitset<>::block_type;
    std::vector<block_type> tombstone_blocks;
    boost::to_block_range(index->node_tombstones, std::back_inserter(tombstone_blocks));
    ofs.write(
        reinterpret_cast<const char*>(tombstone_blocks.data()),
        sizeof(block_type) * tombstone_blocks.size()
    );
    if (!ofs.good()) {
        throw std::runtime_error("Could not write index file");
    }
}

HNSWIndex* HNSWIndexManager::get_hnsw_index(const std::string& name)
{
    std::shared_lock lck(name2hnsw_index_mutex);
    auto it = name2hnsw_index.find(name);
    if (it == name2hnsw_index.end()) {
        return nullptr;
    }

    return it->second.get();
}

template <Catalog::ModelID model_id>
std::tuple<uint_fast32_t> HNSWIndexManager::create_hnsw_index(
    const std::string& name,
    const std::string& predicate,
    uint64_t dimension,
    uint64_t max_edges,
    uint64_t num_candidates,
    MetricType metric_type
)
{
    try {
        const auto metric_func = metric_type2metric_func(metric_type);
        auto hnsw_index = HNSWIndex::create(name, dimension, max_edges, num_candidates, metric_func);

        std::tuple<uint_fast32_t> result;
        if constexpr (model_id == Catalog::ModelID::QUAD) {
            result = hnsw_index->index_property(predicate);
        } else if constexpr (model_id == Catalog::ModelID::RDF) {
            result = hnsw_index->index_predicate(predicate);
        } else {
            throw std::runtime_error(
                "Unhandled ModelID at HNSWIndexManager::create_hnsw_index: "
                + std::to_string(static_cast<int>(model_id))
            );
        }

        auto&& [total_inserted_elements] = result;

        // write the index to disk
        write_to_disk(name, hnsw_index.get());
        hnsw_index->has_changes = false;

        std::unique_lock lck(name2hnsw_index_mutex);
        name2hnsw_index[name] = std::move(hnsw_index);
        name2metadata[name] = { metric_type, predicate };
        predicate2names[predicate].emplace_back(name);
        has_changes_ = true;
        return { total_inserted_elements };
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to create HNSW index \"" + name + "\": " + e.what());
    } catch (...) {
        throw std::runtime_error("Failed to create HNSW index \"" + name + "\": Unknown error");
    }
}

template std::tuple<uint_fast32_t> HNSWIndexManager::create_hnsw_index<
    Catalog::ModelID::QUAD>(const std::string&, const std::string&, uint64_t, uint64_t, uint64_t, MetricType);
template std::tuple<uint_fast32_t> HNSWIndexManager::create_hnsw_index<
    Catalog::ModelID::RDF>(const std::string&, const std::string&, uint64_t, uint64_t, uint64_t, MetricType);
template std::tuple<uint_fast32_t> HNSWIndexManager::create_hnsw_index<
    Catalog::ModelID::GQL>(const std::string&, const std::string&, uint64_t, uint64_t, uint64_t, MetricType);
