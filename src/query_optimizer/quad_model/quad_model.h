#pragma once

#include <type_traits>

#include "execution/binding_id_iter/paths/index_provider/path_index/path_index.h"
#include "execution/graph_model.h"
#include "query_optimizer/quad_model/quad_catalog.h"

template <std::size_t N> class BPlusTree;
template <std::size_t N> class RandomAccessTable;

namespace MDB {
    class OpInsert;
}

// Execution modes for in-memory indexes
enum class IndexMode {
    NAIVE,  // Materialize indexes every time
    CACHE   // Store indexes for future use
};

// Path-finding modes for operators
enum class PathMode {
    BFS,  // Breadth-first search
    DFS   // Depth-first search
};

class QuadModel : public GraphModel {
    class Destroyer {
    friend class QuadModel;
    private:
        Destroyer() = default;
    public:
        ~Destroyer();
    };

public:
    std::unique_ptr<RandomAccessTable<3>> edge_table;

    std::unique_ptr<BPlusTree<1>> nodes;

    std::unique_ptr<BPlusTree<2>> node_label;
    std::unique_ptr<BPlusTree<2>> label_node;

    std::unique_ptr<BPlusTree<3>> object_key_value;
    std::unique_ptr<BPlusTree<3>> key_value_object;

    // std::unique_ptr<BPlusTree<2>> from_edge;
    // std::unique_ptr<BPlusTree<2>> to_edge;
    // std::unique_ptr<BPlusTree<2>> type_edge;

    std::unique_ptr<BPlusTree<4>> from_to_type_edge;
    std::unique_ptr<BPlusTree<4>> to_type_from_edge;
    std::unique_ptr<BPlusTree<4>> type_from_to_edge;
    std::unique_ptr<BPlusTree<4>> type_to_from_edge;

    // special cases
    std::unique_ptr<BPlusTree<3>> equal_from_to;      // (from=to,      type, edge)
    std::unique_ptr<BPlusTree<3>> equal_from_type;    // (from=type,    to,   edge)
    std::unique_ptr<BPlusTree<3>> equal_to_type;      // (to=type,      from, edge)
    std::unique_ptr<BPlusTree<2>> equal_from_to_type; // (from=to=type, edge)

    std::unique_ptr<BPlusTree<3>> equal_from_to_inverted;   // (type, from=to,   edge)
    std::unique_ptr<BPlusTree<3>> equal_from_type_inverted; // (to,   from=type, edge)
    std::unique_ptr<BPlusTree<3>> equal_to_type_inverted;   // (from, to=type,   edge)

    // Index mode to use
    IndexMode index_mode = IndexMode::CACHE;

    // Default index type
    Paths::IndexType default_index_type = Paths::IndexType::BTREE;
    Paths::IndexType default_trails_index_type = Paths::IndexType::BTREE;
    bool use_default_index = true;

    // Path mode to use
    PathMode path_mode = PathMode::BFS;

    // necessary to be called before first usage
    static QuadModel::Destroyer init(const std::string& db_folder,
                                     uint_fast32_t      shared_buffer_pool_size,
                                     uint_fast32_t      private_buffer_pool_size,
                                     uint_fast32_t      max_threads,
                                     const std::string& path_mode,
                                     const std::string& index_mode,
                                     const std::string& index_type);

    std::unique_ptr<BindingIter> exec(Op&, ThreadInfo*) const override;

    void exec_inserts(const MDB::OpInsert&);

    ObjectId get_object_id(const QueryElement&) const override;

    ObjectId get_or_create_object_id(const QueryElement&);

    GraphObject get_graph_object(ObjectId) const override;

    inline QuadCatalog& catalog() const noexcept {
        return const_cast<QuadCatalog&>(reinterpret_cast<const QuadCatalog&>(catalog_buf));
    }

    // Store in-memory indexes for each transition that uses one
    std::unordered_map<uint64_t, std::unique_ptr<Paths::Index>> idx_map;
    std::unordered_map<uint64_t, std::unique_ptr<Paths::Index>> idx_inv_map;

    // Obtain in-memory index for a specific transition
    Paths::Index* get_index(uint64_t type_id, bool inverse, Paths::IndexType idx_type, bool* int_req);
    void save_index_stats(size_t prefix_lvl_size, size_t data_lvl_size, float creation_time);
    Paths::Index* materialize_index(uint64_t type_id, bool inverse, Paths::IndexType idx_type, bool* int_req);

private:
    typename std::aligned_storage<sizeof(QuadCatalog), alignof(QuadCatalog)>::type catalog_buf;

    QuadModel(const std::string& db_folder,
              uint_fast32_t      shared_buffer_pool_size,
              uint_fast32_t      private_buffer_pool_size,
              uint_fast32_t      max_threads,
              const std::string& s_path_mode,
              const std::string& s_index_mode,
              const std::string& s_index_type);

    ~QuadModel();

    void try_add_node(ObjectId);
};

extern QuadModel& quad_model; // global object
