#include "directed_edge_plan.h"

#include "graph_models/gql/gql_model.h"
#include "query/executor/binding_iter/edge_table_lookup_gql.h"
#include "query/executor/binding_iter/index_scan.h"

using namespace GQL;

DirectedEdgePlan::DirectedEdgePlan(Id edge, Id from, Id to) :
    edge(edge),
    from(from),
    to(to),
    edge_assigned(edge.is_OID()),
    from_assigned(from.is_OID()),
    to_assigned(to.is_OID())
{ }

double DirectedEdgePlan::estimate_cost() const
{
    return estimate_output_size();
}

double DirectedEdgePlan::estimate_output_size() const
{
    return gql_model.catalog.directed_edges_count;
}

std::set<VarId> DirectedEdgePlan::get_vars() const
{
    std::set<VarId> result;
    if (from.is_var() && !from_assigned) {
        result.insert(from.get_var());
    }
    if (to.is_var() && !to_assigned) {
        result.insert(to.get_var());
    }
    if (edge.is_var() && !edge_assigned) {
        result.insert(edge.get_var());
    }
    return result;
}

void DirectedEdgePlan::set_input_vars(const std::set<VarId>& input_vars)
{
    set_input_var(input_vars, from, &from_assigned);
    set_input_var(input_vars, to, &to_assigned);
    set_input_var(input_vars, edge, &edge_assigned);
}

// from     edge    to      index
// 1        0       1       tfe
// 1        0       0       fte
// 0        0       1       tfe
// 0        0       0       fte

std::unique_ptr<BindingIter> DirectedEdgePlan::get_binding_iter() const
{
    if (edge_assigned) {
        return std::make_unique<EdgeTableLookupGQL>(
            *gql_model.directed_edges,
            edge.get_var(),
            from,
            to,
            from_assigned,
            to_assigned,
            ObjectId::MASK_DIRECTED_EDGE
        );
    }

    if (from == to) {
        std::array<std::unique_ptr<ScanRange>, 2> ranges;
        ranges[0] = ScanRange::get(from, from_assigned);
        ranges[1] = ScanRange::get(edge, edge_assigned);
        return std::make_unique<IndexScan<2>>(*gql_model.equal_d_edge, std::move(ranges));
    }

    std::array<std::unique_ptr<ScanRange>, 3> ranges;
    if (to_assigned) {
        ranges[0] = ScanRange::get(to, to_assigned);
        ranges[1] = ScanRange::get(from, from_assigned);
        ranges[2] = ScanRange::get(edge, edge_assigned);
        return std::make_unique<IndexScan<3>>(*gql_model.to_from_edge, std::move(ranges));
    } else {
        ranges[0] = ScanRange::get(from, from_assigned);
        ranges[1] = ScanRange::get(to, to_assigned);
        ranges[2] = ScanRange::get(edge, edge_assigned);
        return std::make_unique<IndexScan<3>>(*gql_model.from_to_edge, std::move(ranges));
    }
}

bool DirectedEdgePlan::get_leapfrog_iter(
    std::vector<std::unique_ptr<LeapfrogIter>>& /* leapfrog_iters */,
    std::vector<VarId>& /* var_order */,
    uint_fast32_t& /* enumeration_level */
) const
{
    return false;
}

void DirectedEdgePlan::print(std::ostream& os, int indent) const
{
    os << std::string(indent, ' ');
    os << "DirectedEdge(";
    os << "from: " << from;
    os << ", to: " << to;
    os << ", edge: " << edge;
    os << ")";
}
