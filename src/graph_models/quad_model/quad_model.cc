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


// TODO: create executor to do this
void QuadModel::exec_inserts(const MQL::OpInsert& /*op_insert*/) {
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
    } catch (const LogicException& e) {
        // do nothing
    }
}
