#include "quad_model.h"

#include "query/executor/binding_iter/binding_expr/mql_binding_expr_printer.h"
#include "query/executor/query_executor/mql/return_executor.h"
#include "storage/index/bplus_tree/bplus_tree.h"
#include "storage/index/random_access_table/random_access_table.h"

using namespace std;

// memory for the object
static typename std::aligned_storage<sizeof(QuadModel), alignof(QuadModel)>::type quad_model_buf;
// global object
QuadModel& quad_model = reinterpret_cast<QuadModel&>(quad_model_buf);


std::unique_ptr<ModelDestroyer> QuadModel::init()
{
    new (&quad_model) QuadModel();
    return std::make_unique<ModelDestroyer>([]() { quad_model.~QuadModel(); });
}


std::unique_ptr<BindingExprPrinter> create_quad_binding_expr_printer(std::ostream& os) {
    return std::make_unique<MQL::BindingExprPrinter>(os);
}


std::ostream& debug_print(std::ostream& os, ObjectId oid) {
    MQL::ReturnExecutor<MQL::ReturnType::CSV>::print(os, oid);
    return os;
}


QuadModel::QuadModel() :
    m_catalog("catalog.dat")
{
    QueryContext::_debug_print = debug_print;
    QueryContext::create_binding_expr_printer = create_quad_binding_expr_printer;

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
