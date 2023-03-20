#include "quad_model.h"

#include <iostream>
#include <new>

#include "base/exceptions.h"
#include "base/graph_object/anonymous_node.h"
#include "base/graph_object/edge.h"
#include "execution/graph_object/graph_object_factory.h"
#include "execution/binding_id_iter/paths/path_manager.h"
#include "execution/graph_object/graph_object_manager.h"
#include "query_optimizer/quad_model/binding_iter_visitor.h"
#include "query_optimizer/quad_model/query_element_to_object_id.h"
#include "storage/buffer_manager.h"
#include "storage/file_manager.h"
#include "storage/index/bplus_tree/bplus_tree.h"
#include "storage/index/random_access_table/random_access_table.h"

using namespace std;

// memory for the object
static typename std::aligned_storage<sizeof(QuadModel), alignof(QuadModel)>::type quad_model_buf;
// global object
QuadModel& quad_model = reinterpret_cast<QuadModel&>(quad_model_buf);


QuadModel::Destroyer QuadModel::init(const std::string& db_folder,
                                     uint_fast32_t      shared_buffer_pool_size,
                                     uint_fast32_t      private_buffer_pool_size,
                                     uint_fast32_t      max_threads,
                                     const std::string& path_mode,
                                     const std::string& index_mode,
                                     const std::string& index_type)
{
    new (&quad_model) QuadModel(db_folder,
                                shared_buffer_pool_size,
                                private_buffer_pool_size,
                                max_threads,
                                path_mode,
                                index_mode,
                                index_type);
    return QuadModel::Destroyer();
}


QuadModel::Destroyer::~Destroyer() {
    quad_model.~QuadModel();
}


QuadModel::QuadModel(const std::string& db_folder,
                     uint_fast32_t shared_buffer_pool_size,
                     uint_fast32_t private_buffer_pool_size,
                     uint_fast32_t max_threads,
                     const std::string& s_path_mode,
                     const std::string& s_index_mode,
                     const std::string& s_index_type)
{
    // Set flag for path mode
    if (s_path_mode == "bfs") {
        path_mode = PathMode::BFS;
    } else if (s_path_mode == "dfs"){
        path_mode = PathMode::DFS;
    } else {
        path_mode = PathMode::BFS;
        cout << s_path_mode << " is an invalid path mode!\n";
    }

    // Set flag for index mode
    if (s_index_mode == "cache") {
        index_mode = IndexMode::CACHE;
    } else if (s_index_mode == "naive") {
        index_mode = IndexMode::NAIVE;
    } else {
        index_mode = IndexMode::CACHE;
        cout << s_index_mode << " is an invalid index mode!\n";
    }

    // Set flag for index type
    if (s_index_type == "trie") {
        default_index_type = Paths::IndexType::TRIE;
        default_trails_index_type = Paths::IndexType::EDGE_TRIE;
        use_default_index = true;
    } else if (s_index_type == "btree"){
        default_index_type = Paths::IndexType::BTREE;
        default_trails_index_type = Paths::IndexType::BTREE;
        use_default_index = true;
    } else {
        default_index_type = Paths::IndexType::BTREE;
        default_trails_index_type = Paths::IndexType::BTREE;
        use_default_index = true;
        cout << s_index_type << " is an invalid index type!\n";
    }

    FileManager::init(db_folder);
    BufferManager::init(shared_buffer_pool_size, private_buffer_pool_size, max_threads);
    PathManager::init(max_threads);
    StringManager::init();

    new (&catalog()) QuadCatalog("catalog.dat"); // placement new

    Path::path_printer = &path_manager;

    GraphObject::graph_object_print    = GraphObjectManager::print;
    GraphObject::graph_object_eq       = GraphObjectManager::equal;
    GraphObject::graph_object_cmp      = GraphObjectManager::compare;
    GraphObject::graph_object_sum      = GraphObjectManager::sum;
    GraphObject::graph_object_minus    = GraphObjectManager::minus;
    GraphObject::graph_object_multiply = GraphObjectManager::multiply;
    GraphObject::graph_object_divide   = GraphObjectManager::divide;
    GraphObject::graph_object_modulo   = GraphObjectManager::modulo;

    nodes = make_unique<BPlusTree<1>>("nodes");
    edge_table = make_unique<RandomAccessTable<3>>("edges.table");

    // Create BPT
    label_node = make_unique<BPlusTree<2>>("label_node");
    node_label = make_unique<BPlusTree<2>>("node_label");

    object_key_value = make_unique<BPlusTree<3>>("object_key_value");
    key_value_object = make_unique<BPlusTree<3>>("key_value_object");

    from_to_type_edge = make_unique<BPlusTree<4>>("from_to_type_edge");
    to_type_from_edge = make_unique<BPlusTree<4>>("to_type_from_edge");
    type_from_to_edge = make_unique<BPlusTree<4>>("type_from_to_edge");
    type_to_from_edge = make_unique<BPlusTree<4>>("type_to_from_edge");

    equal_from_to      = make_unique<BPlusTree<3>>("equal_from_to");
    equal_from_type    = make_unique<BPlusTree<3>>("equal_from_type");
    equal_to_type      = make_unique<BPlusTree<3>>("equal_to_type");
    equal_from_to_type = make_unique<BPlusTree<2>>("equal_from_to_type");

    equal_from_to_inverted   = make_unique<BPlusTree<3>>("equal_from_to_inverted");
    equal_from_type_inverted = make_unique<BPlusTree<3>>("equal_from_type_inverted");
    equal_to_type_inverted   = make_unique<BPlusTree<3>>("equal_to_type_inverted");
}


QuadModel::~QuadModel() {
    catalog().save_changes(); // TODO: only if modified?
    catalog().~QuadCatalog();

    nodes.reset();
    edge_table.reset();

    label_node.reset();
    node_label.reset();

    object_key_value.reset();
    key_value_object.reset();

    from_to_type_edge.reset();
    to_type_from_edge.reset();
    type_from_to_edge.reset();
    type_to_from_edge.reset();

    equal_from_to.reset();
    equal_from_type.reset();
    equal_to_type.reset();
    equal_from_to_type.reset();

    equal_from_to_inverted.reset();
    equal_from_type_inverted.reset();
    equal_to_type_inverted.reset();

    string_manager.~StringManager();
    path_manager.~PathManager();
    buffer_manager.~BufferManager();
    file_manager.~FileManager();
}


std::unique_ptr<BindingIter> QuadModel::exec(Op& op, ThreadInfo* thread_info) const {
    auto vars = op.get_vars();
    auto query_optimizer = BindingIterVisitor(std::move(vars), thread_info);
    op.accept_visitor(query_optimizer);
    return move(query_optimizer.tmp);
}


GraphObject QuadModel::get_graph_object(ObjectId object_id) const {
    if ( object_id.is_not_found() ) {
        return GraphObjectFactory::make_not_found();
    }
    if ( object_id.is_null() ) {
        return GraphObjectFactory::make_null();
    }
    auto mask        = object_id.id & ObjectId::TYPE_MASK;
    auto unmasked_id = object_id.id & ObjectId::VALUE_MASK;
    switch (mask) {
        case ObjectId::MASK_STRING_EXTERN : {
            return GraphObjectFactory::make_string_external(unmasked_id);
        }

        case ObjectId::MASK_STRING_INLINED : {
            char c[8];
            int shift_size = 6*8;
            for (int i = 0; i < ObjectId::MAX_INLINED_BYTES; i++) {
                uint8_t byte = (object_id.id >> shift_size) & 0xFF;
                c[i] = byte;
                shift_size -= 8;
            }
            c[7] = '\0';
            return GraphObjectFactory::make_string_inlined(c);
        }

        case ObjectId::MASK_POSITIVE_INT : {
            static_assert(sizeof(int64_t) == 8, "int64_t must be 8 bytes");
            int64_t i = object_id.id & 0x00FF'FFFF'FFFF'FFFFUL;
            return GraphObjectFactory::make_int(i);
        }

        case ObjectId::MASK_NEGATIVE_INT : {
            static_assert(sizeof(int64_t) == 8, "int64_t must be 8 bytes");
            int64_t i = (~object_id.id) & 0x00FF'FFFF'FFFF'FFFFUL;
            return GraphObjectFactory::make_int(i*-1);
        }

        case ObjectId::MASK_FLOAT : {
            static_assert(sizeof(float) == 4, "float must be 4 bytes");
            float f;
            uint8_t* dest = (uint8_t*)&f;
            dest[0] =  object_id.id        & 0xFF;
            dest[1] = (object_id.id >> 8)  & 0xFF;
            dest[2] = (object_id.id >> 16) & 0xFF;
            dest[3] = (object_id.id >> 24) & 0xFF;
            return GraphObjectFactory::make_float(f);
        }

        case ObjectId::MASK_BOOL : {
            bool b;
            uint8_t* dest = (uint8_t*)&b;
            *dest = object_id.id & 0xFF;
            return GraphObjectFactory::make_bool(b);
        }

        case ObjectId::MASK_NAMED_NODE_EXTERN : {
            return GraphObjectFactory::make_named_node_external(unmasked_id);
        }

        case ObjectId::MASK_NAMED_NODE_INLINED : {
            char c[8];
            int shift_size = 6*8;
            for (int i = 0; i < ObjectId::MAX_INLINED_BYTES; i++) {
                uint8_t byte = (object_id.id >> shift_size) & 0xFF;
                c[i] = byte;
                shift_size -= 8;
            }
            c[7] = '\0';
            return GraphObjectFactory::make_named_node_inlined(c);
        }

        case ObjectId::MASK_ANON : {
            return GraphObjectFactory::make_anonymous(unmasked_id);
        }

        case ObjectId::MASK_EDGE : {
            return GraphObjectFactory::make_edge(unmasked_id);
        }

        case ObjectId::MASK_PATH : {
            return GraphObjectFactory::make_path(unmasked_id);
        }

        default : {
            throw LogicException("Unhandled Object Type.");
        }
    }
}


ObjectId QuadModel::get_object_id(const QueryElement& query_element) const {
    QueryElementToObjectId visitor;
    return visitor(query_element);
}


ObjectId QuadModel::get_or_create_object_id(const QueryElement& query_element) {
    QueryElementToObjectId visitor(true);
    return visitor(query_element);
}


void QuadModel::exec_inserts(const MDB::OpInsert& op_insert) {
    for (auto& op_label : op_insert.labels) {
        auto label_id = get_or_create_object_id(QueryElement(op_label.label));
        auto node_id = get_or_create_object_id(op_label.node);

        try_add_node(node_id);

        label_node->insert(Record<2>({label_id.id, node_id.id}));
        node_label->insert(Record<2>({node_id.id, label_id.id}));

        catalog().label_count++;
        catalog().label2total_count[label_id.id]++;
        catalog().distinct_labels = catalog().label2total_count.size();
    }

    for (auto& op_property : op_insert.properties) {
        auto obj_id = get_or_create_object_id(op_property.node);
        auto key_id = get_or_create_object_id(QueryElement(op_property.key));
        auto val_id = get_or_create_object_id(op_property.value);

        try_add_node(obj_id);

        key_value_object->insert(Record<3>({key_id.id, val_id.id, obj_id.id}));
        object_key_value->insert(Record<3>({obj_id.id, key_id.id, val_id.id}));

        catalog().properties_count++;
        catalog().key2total_count[key_id.id]++;

        {
            bool interruption_requested = false;
            auto iter = key_value_object->get_range(&interruption_requested,
                                                    Record<3>({key_id.id, val_id.id, 0}),
                                                    Record<3>({key_id.id, val_id.id, UINT64_MAX}));
            if (iter->next() != nullptr) {
                catalog().key2distinct[key_id.id]++;
            }
        }


        catalog().distinct_keys = catalog().key2total_count.size();
    }

    // insert edges
    for (auto& op_edge : op_insert.edges) {
        auto from = get_or_create_object_id(op_edge.from);
        auto to   = get_or_create_object_id(op_edge.to);
        auto type = get_or_create_object_id(op_edge.type);
        // TODO: reuse edge_ids from recycler when deletes are implemented
        auto edge_id = ++catalog().connections_count | ObjectId::MASK_EDGE;

        try_add_node(from);
        try_add_node(to);
        try_add_node(type);

        type_from_to_edge->insert(Record<4>({type.id, from.id, to.id, edge_id}));
        type_to_from_edge->insert(Record<4>({type.id, to.id, from.id, edge_id}));
        from_to_type_edge->insert(Record<4>({from.id, to.id, type.id, edge_id}));
        to_type_from_edge->insert(Record<4>({to.id, type.id, from.id, edge_id}));

        edge_table->append_record(Record<3>({from.id, to.id, type.id}));

        catalog().connections_count++;
        catalog().type2total_count[type.id]++;
        catalog().distinct_type = catalog().type2total_count.size();

        if (from == to) {
            equal_from_to->insert(Record<3>({from.id, type.id, edge_id}));
            equal_from_to_inverted->insert(Record<3>({type.id, from.id, edge_id}));
            catalog().equal_from_to_count++;
            catalog().type2equal_from_to_count[type.id]++;
            if (from == type) {
                equal_from_to_type->insert(Record<2>({from.id, edge_id}));
                catalog().equal_from_to_type_count++;
                catalog().type2equal_from_to_type_count[type.id]++;
            }
        }
        if (from == type) {
            equal_from_type->insert(Record<3>({from.id, to.id, edge_id}));
            equal_from_type_inverted->insert(Record<3>({to.id, from.id, edge_id}));
            catalog().equal_from_type_count++;
            catalog().type2equal_from_type_count[type.id]++;
        }
        if (type == to) {
            equal_to_type->insert(Record<3>({to.id, from.id, edge_id}));
            equal_to_type_inverted->insert(Record<3>({from.id, to.id, edge_id}));
            catalog().equal_to_type_count++;
            catalog().type2equal_to_type_count[type.id]++;
        }
    }
}


void QuadModel::try_add_node(ObjectId node_id) {
    try {
        // will throw exception if node exists
        nodes->insert(Record<1>({node_id.id}));

        if ((node_id.id & ObjectId::TYPE_MASK) == ObjectId::MASK_ANON) {
            catalog().anonymous_nodes_count++;
        } else {
            catalog().identifiable_nodes_count++;
        }
    } catch (const LogicException& e) { // TODO: use custom exception?
        // do nothing
    }
}


Paths::Index* QuadModel::get_index(uint64_t type_id, bool inverse, Paths::IndexType idx_type, bool* int_req) {
    if (inverse) {
        auto assigned_index = idx_inv_map.find(type_id);
        if (assigned_index == idx_inv_map.end()) {
            return materialize_index(type_id, inverse, idx_type, int_req);
        } else {
            return assigned_index->second.get();
        }
    } else {
        auto assigned_index = idx_map.find(type_id);
        if (assigned_index == idx_map.end()) {
            return materialize_index(type_id, inverse, idx_type, int_req);
        } else {
            return assigned_index->second.get();
        }
    }
}


void QuadModel::save_index_stats(size_t prefix_lvl_size, size_t data_lvl_size, float creation_time) {
    fstream stats_file;
    stats_file.open("trie_stats.csv", ios::app);
    stats_file << prefix_lvl_size << "," << data_lvl_size << "," << creation_time << ";";
    stats_file.close();
}


Paths::Index* QuadModel::materialize_index(uint64_t type_id, bool inverse, Paths::IndexType idx_type, bool* int_req) {
    // All results for the specified transition type
    array<uint64_t, 4> min_ids;
    array<uint64_t, 4> max_ids;
    min_ids[0] = type_id;
    max_ids[0] = type_id;
    min_ids[1] = 0;
    max_ids[1] = UINT64_MAX;
    min_ids[2] = 0;
    max_ids[2] = UINT64_MAX;
    min_ids[3] = 0;
    max_ids[3] = UINT64_MAX;

    // Timer for index creation
    auto index_creation_start = chrono::system_clock::now();
    chrono::duration<float, std::nano> index_creation_time;

    // Construct index
    if (inverse) {
        auto aux_iter = quad_model.type_to_from_edge->get_range(int_req,
                                                                Record<4>(min_ids),
                                                                Record<4>(max_ids));
        if (idx_type == Paths::IndexType::TRIE) {
            auto type_map = make_unique<Trie>();
            for (auto record = aux_iter->next(); record != nullptr; record = aux_iter->next()) {
                type_map->insert(record->ids[1], record->ids[2]);
            }
            type_map->end_inserts();

            // Write Trie statistics to file
            index_creation_time = chrono::system_clock::now() - index_creation_start;
            save_index_stats(type_map->prefix_lvl.size(), type_map->data_lvl.size(), index_creation_time.count());

            auto idx = make_unique<Paths::TrieIndex>(move(type_map));
            return idx_inv_map.insert({type_id, move(idx)}).first->second.get();
        } else if (idx_type == Paths::IndexType::EDGE_TRIE) {
            auto type_map = make_unique<EdgeTrie>();
            for (auto record = aux_iter->next(); record != nullptr; record = aux_iter->next()) {
                type_map->insert(record->ids[1], record->ids[2], record->ids[3]);
            }
            type_map->end_inserts();

            // Write Trie statistics to file
            index_creation_time = chrono::system_clock::now() - index_creation_start;
            save_index_stats(type_map->prefix_lvl.size(), type_map->data_lvl.size(), index_creation_time.count());

            auto idx = make_unique<Paths::EdgeTrieIndex>(move(type_map));
            return idx_inv_map.insert({type_id, move(idx)}).first->second.get();
        } else {
            auto type_map = make_unique<HashTrie>();
            for (auto record = aux_iter->next(); record != nullptr; record = aux_iter->next()) {
                type_map->insert(record->ids[1], record->ids[2]);
            }
            type_map->end_inserts();

            // Write Trie statistics to file
            index_creation_time = chrono::system_clock::now() - index_creation_start;
            save_index_stats(type_map->offsets.size(), type_map->data_lvl.size(), index_creation_time.count());

            auto idx = make_unique<Paths::HashTrieIndex>(move(type_map));
            return idx_inv_map.insert({type_id, move(idx)}).first->second.get();
        }
    } else {
        auto aux_iter = quad_model.type_from_to_edge->get_range(int_req,
                                                                Record<4>(min_ids),
                                                                Record<4>(max_ids));
        if (idx_type == Paths::IndexType::TRIE) {
            auto type_map = make_unique<Trie>();
            for (auto record = aux_iter->next(); record != nullptr; record = aux_iter->next()) {
                type_map->insert(record->ids[1], record->ids[2]);
            }
            type_map->end_inserts();

            // Write Trie statistics to file
            index_creation_time = chrono::system_clock::now() - index_creation_start;
            save_index_stats(type_map->prefix_lvl.size(), type_map->data_lvl.size(), index_creation_time.count());

            auto idx = make_unique<Paths::TrieIndex>(move(type_map));
            return idx_map.insert({type_id, move(idx)}).first->second.get();
        } else if (idx_type == Paths::IndexType::EDGE_TRIE) {
            auto type_map = make_unique<EdgeTrie>();
            for (auto record = aux_iter->next(); record != nullptr; record = aux_iter->next()) {
                type_map->insert(record->ids[1], record->ids[2], record->ids[3]);
            }
            type_map->end_inserts();

            // Write Trie statistics to file
            index_creation_time = chrono::system_clock::now() - index_creation_start;
            save_index_stats(type_map->prefix_lvl.size(), type_map->data_lvl.size(), index_creation_time.count());

            auto idx = make_unique<Paths::EdgeTrieIndex>(move(type_map));
            return idx_map.insert({type_id, move(idx)}).first->second.get();
        } else {
            auto type_map = make_unique<HashTrie>();
            for (auto record = aux_iter->next(); record != nullptr; record = aux_iter->next()) {
                type_map->insert(record->ids[1], record->ids[2]);
            }
            type_map->end_inserts();

            // Write Trie statistics to file
            index_creation_time = chrono::system_clock::now() - index_creation_start;
            save_index_stats(type_map->offsets.size(), type_map->data_lvl.size(), index_creation_time.count());

            auto idx = make_unique<Paths::HashTrieIndex>(move(type_map));
            return idx_map.insert({type_id, move(idx)}).first->second.get();
        }
    }
}
