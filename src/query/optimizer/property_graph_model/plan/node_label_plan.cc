#include "node_label_plan.h"

#include "graph_models/gql/gql_model.h"
#include "query/executor/binding_iter/index_scan.h"

using namespace GQL;

NodeLabelPlan::NodeLabelPlan(const NodeLabelPlan& other) :
    node(other.node),
    label(other.label),
    node_assigned(other.node_assigned),
    label_assigned(other.label_assigned)
{ }

NodeLabelPlan::NodeLabelPlan(Id node, Id label) :
    node(node),
    label(label),
    node_assigned(node.is_OID()),
    label_assigned(label.is_OID())
{ }

double NodeLabelPlan::estimate_cost() const
{
    return estimate_output_size();
}

double NodeLabelPlan::estimate_output_size() const
{
    return 100;
}

std::set<VarId> NodeLabelPlan::get_vars() const
{
    std::set<VarId> result;
    if (node.is_var() && !node_assigned) {
        result.insert(node.get_var());
    }
    if (label.is_var() && !label_assigned) {
        result.insert(label.get_var());
    }
    return result;
}

void NodeLabelPlan::set_input_vars(const std::set<VarId>& input_vars)
{
    set_input_var(input_vars, node, &node_assigned);
    set_input_var(input_vars, label, &label_assigned);
}

std::unique_ptr<BindingIter> NodeLabelPlan::get_binding_iter() const
{
    std::array<std::unique_ptr<ScanRange>, 2> ranges;
    if (node_assigned) {
        ranges[0] = ScanRange::get(node, node_assigned);
        ranges[1] = ScanRange::get(label, label_assigned);
        return std::make_unique<IndexScan<2>>(*gql_model.node_label, std::move(ranges));
    } else if (label_assigned) {
        ranges[0] = ScanRange::get(label, label_assigned);
        ranges[1] = ScanRange::get(node, node_assigned);
        return std::make_unique<IndexScan<2>>(*gql_model.label_node, std::move(ranges));
    } else {
        ranges[0] = ScanRange::get(node, node_assigned);
        ranges[1] = ScanRange::get(label, label_assigned);
        return std::make_unique<IndexScan<2>>(*gql_model.node_label, std::move(ranges));
    }
}

bool NodeLabelPlan::get_leapfrog_iter(
    std::vector<std::unique_ptr<LeapfrogIter>>& /* leapfrog_iters */,
    std::vector<VarId>& /* var_order */,
    uint_fast32_t& /* enumeration_level */
) const
{
    return false;
}

void NodeLabelPlan::print(std::ostream& os, int indent) const
{
    os << std::string(indent, ' ');
    os << "NodeLabel(";
    os << "node: " << node;
    os << ", label: " << label;
    os << ")";
}
