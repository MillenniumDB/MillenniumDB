#include "node_plan.h"
#include "graph_models/gql/gql_model.h"
#include "query/executor/binding_iter/object_enum.h"
#include "query/executor/binding_iter/scan_ranges/unassigned_var.h"
#include "query/executor/binding_iter/union.h"
#include "query/executor/binding_iter/single_result_binding_iter.h"

using namespace GQL;

double NodePlan::estimate_cost() const
{
    return estimate_output_size();
}

double NodePlan::estimate_output_size() const
{
    return gql_model.catalog.nodes_count;
}

std::set<VarId> NodePlan::get_vars() const
{
    std::set<VarId> result;
    result.insert(node_id);
    return result;
}

void NodePlan::set_input_vars(const std::set<VarId>& input_vars) {
    set_input_var(input_vars, node_id, &node_assigned);
}

std::unique_ptr<BindingIter> NodePlan::get_binding_iter() const
{
    if (node_assigned) {
        return std::make_unique<SingleResultBindingIter>();
    }
    return std::make_unique<ObjectEnum>(node_id, ObjectId::MASK_NODE, gql_model.catalog.nodes_count);
}

void NodePlan::print(std::ostream& os, int indent) const
{
    for (int i = 0; i < indent; ++i) {
        os << ' ';
    }
    os << std::string(indent, ' ');
    os << "NodePlan(" << get_query_ctx().get_var_name(node_id) << ")";
}
