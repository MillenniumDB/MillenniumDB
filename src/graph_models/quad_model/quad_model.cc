#include "quad_model.h"

#include <new>

#include "graph_models/inliner.h"
#include "query/exceptions.h"
#include "query/executor/binding_iter/paths/path_manager.h"
#include "query/executor/query_executor/mql/csv_return_executor.h"
#include "query/optimizer/quad_model/executor_constructor.h"
#include "storage/buffer_manager.h"
#include "storage/file_manager.h"
#include "storage/index/bplus_tree/bplus_tree.h"
#include "storage/index/random_access_table/random_access_table.h"
#include "storage/string_manager.h"
#include "storage/tmp_manager.h"

using namespace std;

// memory for the object
static typename std::aligned_storage<sizeof(QuadModel), alignof(QuadModel)>::type quad_model_buf;
// global object
QuadModel& quad_model = reinterpret_cast<QuadModel&>(quad_model_buf);


QuadModel::Destroyer QuadModel::init(const std::string& db_folder,
                                     uint_fast32_t      str_max_initial_populate_size,
                                     uint_fast32_t      shared_buffer_pool_size,
                                     uint_fast32_t      private_buffer_pool_size,
                                     uint_fast32_t      max_threads)
{
    new (&quad_model) QuadModel(db_folder,
                                str_max_initial_populate_size,
                                shared_buffer_pool_size,
                                private_buffer_pool_size,
                                max_threads);
    return QuadModel::Destroyer();
}


QuadModel::Destroyer::~Destroyer() {
    quad_model.~QuadModel();
}


std::ostream& debug_print(std::ostream& os, ObjectId oid) {
    MQL::CSVReturnExecutor::print(os, oid);
    return os;
}

QuadModel::QuadModel(const std::string& db_folder,
                     uint_fast32_t str_max_initial_populate_size,
                     uint_fast32_t shared_buffer_pool_size,
                     uint_fast32_t private_buffer_pool_size,
                     uint_fast32_t max_threads)
{
    FileManager::init(db_folder);
    BufferManager::init(shared_buffer_pool_size, private_buffer_pool_size, max_threads);
    PathManager::init(max_threads);
    StringManager::init(str_max_initial_populate_size);
    TmpManager::init(max_threads);

    QueryContext::_debug_print = debug_print;

    new (&catalog()) QuadCatalog("catalog.dat"); // placement new

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

    tmp_manager.~TmpManager();
    string_manager.~StringManager();
    path_manager.~PathManager();
    buffer_manager.~BufferManager();
    file_manager.~FileManager();
}


// ObjectId get_or_create_object_id(Id id) {
//     // TODO: transform TMPs into real ids
//     // should be done here or before (query_parser)?
//     assert(id.is_OID());
//     auto oid = id.get_OID();
//     const auto mask        = oid.id & ObjectId::TYPE_MASK;
//     const auto unmasked_id = oid.id & ObjectId::VALUE_MASK;
//     switch (mask) {

//     // TODO: implement ObjectId::MASK_ANON_TMP if supported in the future
//     // case ObjectId::MASK_ANON_TMP:
//     case ObjectId::MASK_NAMED_NODE_TMP: {
//         tmp_manager.print_str(os, unmasked_id);
//         break;
//     }
//     case ObjectId::MASK_STRING_SIMPLE_TMP: {
//         os << '"';
//         tmp_manager.print_str(escaped_os, unmasked_id);
//         os << '"';
//         break;
//     }

//     default:
//         throw std::logic_error("Unmanaged mask in CSVReturnExecutor print: "
//             + std::to_string(mask));
//     }
// }


// TODO: create executor to do this
void QuadModel::exec_inserts(const MQL::OpInsert& op_insert) {
    // for (auto& op_label : op_insert.labels) {
    //     auto label_id = get_or_create_object_id(op_label.label);
    //     auto node_id  = get_or_create_object_id(op_label.node);

    //     try_add_node(node_id);

    //     label_node->insert(Record<2>({label_id.id, node_id.id}));
    //     node_label->insert(Record<2>({node_id.id, label_id.id}));

    //     catalog().label_count++;
    //     catalog().label2total_count[label_id.id]++;
    //     catalog().distinct_labels = catalog().label2total_count.size();
    // }

    // for (auto& op_property : op_insert.properties) {
    //     auto obj_id = get_or_create_object_id(op_property.node);
    //     auto key_id = get_or_create_object_id(op_property.key);
    //     auto val_id = get_or_create_object_id(op_property.value);

    //     try_add_node(obj_id);

    //     key_value_object->insert(Record<3>({key_id.id, val_id.id, obj_id.id}));
    //     object_key_value->insert(Record<3>({obj_id.id, key_id.id, val_id.id}));

    //     catalog().properties_count++;
    //     catalog().key2total_count[key_id.id]++;

    //     {
    //         bool interruption_requested = false;
    //         auto iter = key_value_object->get_range(&interruption_requested,
    //                                                 Record<3>({key_id.id, val_id.id, 0}),
    //                                                 Record<3>({key_id.id, val_id.id, UINT64_MAX}));
    //         if (iter.next() != nullptr) {
    //             catalog().key2distinct[key_id.id]++;
    //         }
    //     }


    //     catalog().distinct_keys = catalog().key2total_count.size();
    // }

    // // insert edges
    // for (auto& op_edge : op_insert.edges) {
    //     auto from = get_or_create_object_id(op_edge.from);
    //     auto to   = get_or_create_object_id(op_edge.to);
    //     auto type = get_or_create_object_id(op_edge.type);
    //     // TODO: reuse edge_ids from recycler when deletes are implemented
    //     auto edge_id = ++catalog().connections_count | ObjectId::MASK_EDGE;

    //     try_add_node(from);
    //     try_add_node(to);
    //     try_add_node(type);

    //     type_from_to_edge->insert(Record<4>({type.id, from.id, to.id, edge_id}));
    //     type_to_from_edge->insert(Record<4>({type.id, to.id, from.id, edge_id}));
    //     from_to_type_edge->insert(Record<4>({from.id, to.id, type.id, edge_id}));
    //     to_type_from_edge->insert(Record<4>({to.id, type.id, from.id, edge_id}));

    //     edge_table->append_record(Record<3>({from.id, to.id, type.id}));

    //     catalog().type2total_count[type.id]++;
    //     catalog().distinct_type = catalog().type2total_count.size();

    //     if (from == to) {
    //         equal_from_to->insert(Record<3>({from.id, type.id, edge_id}));
    //         equal_from_to_inverted->insert(Record<3>({type.id, from.id, edge_id}));
    //         catalog().equal_from_to_count++;
    //         catalog().type2equal_from_to_count[type.id]++;
    //         if (from == type) {
    //             equal_from_to_type->insert(Record<2>({from.id, edge_id}));
    //             catalog().equal_from_to_type_count++;
    //             catalog().type2equal_from_to_type_count[type.id]++;
    //         }
    //     }
    //     if (from == type) {
    //         equal_from_type->insert(Record<3>({from.id, to.id, edge_id}));
    //         equal_from_type_inverted->insert(Record<3>({to.id, from.id, edge_id}));
    //         catalog().equal_from_type_count++;
    //         catalog().type2equal_from_type_count[type.id]++;
    //     }
    //     if (type == to) {
    //         equal_to_type->insert(Record<3>({to.id, from.id, edge_id}));
    //         equal_to_type_inverted->insert(Record<3>({from.id, to.id, edge_id}));
    //         catalog().equal_to_type_count++;
    //         catalog().type2equal_to_type_count[type.id]++;
    //     }
    // }
}


void QuadModel::try_add_node(ObjectId node_id) {
    try {
        // will throw exception if node exists
        nodes->insert(Record<1>({node_id.id}));

        if ((node_id.id & ObjectId::TYPE_MASK) == ObjectId::MASK_ANON_INLINED) {
            catalog().anonymous_nodes_count++;
        } else {
            catalog().identifiable_nodes_count++;
        }
    } catch (const LogicException& e) { // TODO: use custom exception?
        // do nothing
    }
}
