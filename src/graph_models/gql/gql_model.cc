#include "gql_model.h"

#include <type_traits>

#include "graph_models/gql/conversions.h"
#include "query/executor/binding_iter/binding_expr/gql_binding_expr_printer.h"
#include "query/query_context.h"
#include "storage/index/bplus_tree/bplus_tree.h"
#include "storage/index/random_access_table/random_access_table.h"

// memory for the object
static typename std::aligned_storage<sizeof(GQLModel), alignof(GQLModel)>::type gql_model_buf;

// global object
GQLModel& gql_model = reinterpret_cast<GQLModel&>(gql_model_buf);

std::unique_ptr<ModelDestroyer> GQLModel::init()
{
    new (&gql_model) GQLModel();
    return std::make_unique<ModelDestroyer>([]() { gql_model.~GQLModel(); });
}

std::unique_ptr<BindingExprPrinter> create_gql_binding_expr_printer(std::ostream& os)
{
    return std::make_unique<GQL::BindingExprPrinter>(os);
}

GQLModel::GQLModel() :
    catalog("catalog.dat")
{
    QueryContext::_debug_print = GQL::Conversions::debug_print;
    QueryContext::create_binding_expr_printer = create_gql_binding_expr_printer;

    directed_edges = std::make_unique<RandomAccessTable<2>>("d_edges.table");
    undirected_edges = std::make_unique<RandomAccessTable<2>>("u_edges.table");
    node_label = std::make_unique<BPlusTree<2>>("node_label");
    label_node = std::make_unique<BPlusTree<2>>("label_node");
    edge_label = std::make_unique<BPlusTree<2>>("edge_label");
    label_edge = std::make_unique<BPlusTree<2>>("label_edge");
    node_key_value = std::make_unique<BPlusTree<3>>("node_key_value");
    key_value_node = std::make_unique<BPlusTree<3>>("key_value_node");
    edge_key_value = std::make_unique<BPlusTree<3>>("edge_key_value");
    key_value_edge = std::make_unique<BPlusTree<3>>("key_value_edge");
    from_to_edge = std::make_unique<BPlusTree<3>>("from_to_edge");
    to_from_edge = std::make_unique<BPlusTree<3>>("to_from_edge");
    u_edge = std::make_unique<BPlusTree<3>>("u_edge");
    equal_u_edge = std::make_unique<BPlusTree<2>>("equal_u_edge");
    equal_d_edge = std::make_unique<BPlusTree<2>>("equal_d_edge");
}
