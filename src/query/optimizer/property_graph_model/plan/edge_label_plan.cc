#include "edge_label_plan.h"

#include "graph_models/gql/gql_model.h"
#include "query/executor/binding_iter/index_scan.h"

using namespace GQL;

EdgeLabelPlan::EdgeLabelPlan(Id edge, Id label) :
    edge(edge),
    label(label),
    edge_assigned(edge.is_OID()),
    label_assigned(label.is_OID())
{ }

EdgeLabelPlan::EdgeLabelPlan(const EdgeLabelPlan& other) :
    edge(other.edge),
    label(other.label),
    edge_assigned(other.edge_assigned),
    label_assigned(other.label_assigned)
{ }

double EdgeLabelPlan::estimate_cost() const
{
    return estimate_output_size();
}

double EdgeLabelPlan::estimate_output_size() const
{
    return 100;
}

std::set<VarId> EdgeLabelPlan::get_vars() const
{
    std::set<VarId> result;
    if (edge.is_var() && !edge_assigned) {
        result.insert(edge.get_var());
    }
    if (label.is_var() && !label_assigned) {
        result.insert(label.get_var());
    }
    return result;
}

void EdgeLabelPlan::set_input_vars(const std::set<VarId>& input_vars)
{
    set_input_var(input_vars, label, &label_assigned);
    set_input_var(input_vars, edge, &edge_assigned);
}

std::unique_ptr<BindingIter> EdgeLabelPlan::get_binding_iter() const
{
    std::array<std::unique_ptr<ScanRange>, 2> ranges;
    if (edge_assigned) {
        ranges[0] = ScanRange::get(edge, edge_assigned);
        ranges[1] = ScanRange::get(label, label_assigned);
        return std::make_unique<IndexScan<2>>(*gql_model.edge_label, std::move(ranges));
    } else if (label_assigned) {
        ranges[0] = ScanRange::get(label, label_assigned);
        ranges[1] = ScanRange::get(edge, edge_assigned);
        return std::make_unique<IndexScan<2>>(*gql_model.label_edge, std::move(ranges));
    } else {
        ranges[0] = ScanRange::get(edge, edge_assigned);
        ranges[1] = ScanRange::get(label, label_assigned);
        return std::make_unique<IndexScan<2>>(*gql_model.edge_label, std::move(ranges));
    }
}

bool EdgeLabelPlan::get_leapfrog_iter(
    std::vector<std::unique_ptr<LeapfrogIter>>& /* leapfrog_iters */,
    std::vector<VarId>& /* var_order */,
    uint_fast32_t& /* enumeration_level */
) const
{
    return false;
}

void EdgeLabelPlan::print(std::ostream& os, int indent) const
{
    os << std::string(indent, ' ');
    os << "EdgeLabel(";
    os << "edge: " << edge;
    os << ", label: " << label;
    os << ")";
}
