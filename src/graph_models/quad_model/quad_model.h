#pragma once

#include <type_traits>

#include "graph_models/object_id.h"
#include "graph_models/quad_model/quad_catalog.h"
#include "query/parser/op/op.h"
#include "query/parser/paths/regular_path_expr.h"
#include "query/query_context.h"

template <std::size_t N> class BPlusTree;
template <std::size_t N> class RandomAccessTable;

namespace MQL {
    class OpInsert;
}

class QuadModel {
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

    uint64_t MAX_LIMIT = UINT64_MAX;

    // Path mode to use
    PathMode path_mode = PathMode::BFS;

    // necessary to be called before first usage
    static QuadModel::Destroyer init(const std::string& db_folder,
                                     uint_fast32_t      str_max_initial_populate_size,
                                     uint_fast32_t      shared_buffer_pool_size,
                                     uint_fast32_t      private_buffer_pool_size,
                                     uint_fast32_t      max_threads);


    void exec_inserts(const MQL::OpInsert&);

    inline QuadCatalog& catalog() const noexcept {
        return const_cast<QuadCatalog&>(reinterpret_cast<const QuadCatalog&>(catalog_buf));
    }

private:
    typename std::aligned_storage<sizeof(QuadCatalog), alignof(QuadCatalog)>::type catalog_buf;

    QuadModel(const std::string& db_folder,
              uint_fast32_t      str_max_initial_populate_size,
              uint_fast32_t      shared_buffer_pool_size,
              uint_fast32_t      private_buffer_pool_size,
              uint_fast32_t      max_threads);

    ~QuadModel();

    void try_add_node(ObjectId);
};

extern QuadModel& quad_model; // global object
