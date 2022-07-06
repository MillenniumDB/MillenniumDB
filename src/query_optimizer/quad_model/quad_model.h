#pragma once

#include <type_traits>

#include "execution/graph_model.h"
#include "query_optimizer/quad_model/quad_catalog.h"
#include "storage/index/bplus_tree/bplus_tree.h"
#include "storage/index/hash/object_file_hash/object_file_hash.h"
#include "storage/index/object_file/object_file.h"
#include "storage/index/random_access_table/random_access_table.h"

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

    // necesary to be called before first usage
    static QuadModel::Destroyer init(const std::string& db_folder,
                                     uint_fast32_t      shared_buffer_pool_size,
                                     uint_fast32_t      private_buffer_pool_size,
                                     uint_fast32_t      max_threads);


    std::unique_ptr<BindingIter> exec(Op&, ThreadInfo*) const override;

    ObjectId get_object_id(const GraphObject&) const override;

    GraphObject get_graph_object(ObjectId) const override;

    // Methods used by bulk_import
    uint64_t get_or_create_object_id(const GraphObject&);

    inline QuadCatalog& catalog() const noexcept {
        return const_cast<QuadCatalog&>(reinterpret_cast<const QuadCatalog&>(catalog_buf));
    }

    inline ObjectFile& object_file() const noexcept {
        return const_cast<ObjectFile&>(reinterpret_cast<const ObjectFile&>(object_file_buf));
    }

    inline ObjectFileHash& strings_hash() const noexcept {
        return const_cast<ObjectFileHash&>(reinterpret_cast<const ObjectFileHash&>(strings_cache_buf));
    }

private:
    typename std::aligned_storage<sizeof(QuadCatalog), alignof(QuadCatalog)>::type       catalog_buf;
    typename std::aligned_storage<sizeof(ObjectFile), alignof(ObjectFile)>::type         object_file_buf;
    typename std::aligned_storage<sizeof(ObjectFileHash), alignof(ObjectFileHash)>::type strings_cache_buf;

    QuadModel(const std::string& db_folder,
              uint_fast32_t      shared_buffer_pool_size,
              uint_fast32_t      private_buffer_pool_size,
              uint_fast32_t      max_threads);

    ~QuadModel();
};

extern QuadModel& quad_model; // global object
