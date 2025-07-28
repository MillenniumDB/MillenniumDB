#include "undirected_directed_edge_plan.h"

#include "graph_models/gql/gql_model.h"
#include "query/exceptions.h"
#include "query/executor/binding_iter/edge_direction_left.h"
#include "query/executor/binding_iter/edge_direction_right.h"
#include "query/executor/binding_iter/edge_direction_undirected.h"
#include "query/executor/binding_iter/edge_table_lookup_gql.h"
#include "query/executor/binding_iter/index_scan.h"
#include "query/executor/binding_iter/union.h"

using namespace GQL;

UndirectedDirectedEdgePlan::UndirectedDirectedEdgePlan(Id edge, Id from, Id to, VarId direction_var, ObjectId direction) :
    edge(edge),
    from(from),
    to(to),
    direction_var(direction_var),
    edge_assigned(edge.is_OID()),
    from_assigned(from.is_OID()),
    to_assigned(to.is_OID()),
    direction(direction)
{ }

double UndirectedDirectedEdgePlan::estimate_cost() const
{
    return estimate_output_size();
}

double UndirectedDirectedEdgePlan::estimate_output_size() const
{
    return gql_model.catalog.directed_edges_count + gql_model.catalog.undirected_edges_count;
}

std::set<VarId> UndirectedDirectedEdgePlan::get_vars() const
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

void UndirectedDirectedEdgePlan::set_input_vars(const std::set<VarId>& input_vars)
{
    set_input_var(input_vars, from, &from_assigned);
    set_input_var(input_vars, to, &to_assigned);
    set_input_var(input_vars, edge, &edge_assigned);
}

std::unique_ptr<BindingIter> UndirectedDirectedEdgePlan::get_binding_iter() const
{
    if (edge_assigned) {
        std::unique_ptr<BindingIter> directed = std::make_unique<EdgeTableLookupGQL>(
            *gql_model.directed_edges,
            edge.get_var(),
            from,
            to,
            from_assigned,
            to_assigned,
            ObjectId::MASK_DIRECTED_EDGE
        );
        directed = add_direction_iter(std::move(directed));

        std::unique_ptr<BindingIter> undirected = std::make_unique<EdgeTableLookupGQL>(
            *gql_model.undirected_edges,
            edge.get_var(),
            from,
            to,
            from_assigned,
            to_assigned,
            ObjectId::MASK_UNDIRECTED_EDGE
        );
        undirected = std::make_unique<EdgeDirectionUndirected>(std::move(undirected), direction_var);

        std::vector<std::unique_ptr<BindingIter>> iters;
        iters.push_back(std::move(directed));
        iters.push_back(std::move(undirected));

        return std::make_unique<Union>(std::move(iters));
    }

    if (from == to) {
        std::array<std::unique_ptr<ScanRange>, 2> ranges_undirected;
        ranges_undirected[0] = ScanRange::get(from, from_assigned);
        ranges_undirected[1] = ScanRange::get(edge, edge_assigned);
        std::unique_ptr<BindingIter> undirected = std::make_unique<IndexScan<2>>(
            *gql_model.equal_u_edge,
            std::move(ranges_undirected)
        );
        undirected = std::make_unique<EdgeDirectionUndirected>(std::move(undirected), direction_var);

        std::array<std::unique_ptr<ScanRange>, 2> ranges_directed;
        ranges_directed[0] = ScanRange::get(from, from_assigned);
        ranges_directed[1] = ScanRange::get(edge, edge_assigned);
        std::unique_ptr<BindingIter> directed = std::make_unique<IndexScan<2>>(
            *gql_model.equal_d_edge,
            std::move(ranges_directed)
        );
        directed = add_direction_iter(std::move(directed));

        std::vector<std::unique_ptr<BindingIter>> iters;
        iters.push_back(std::move(directed));
        iters.push_back(std::move(undirected));
        return std::make_unique<Union>(std::move(iters));
    }

    std::array<std::unique_ptr<ScanRange>, 3> ranges;

    std::unique_ptr<BindingIter> undirected;
    if (to_assigned) {
        ranges[0] = ScanRange::get(to, to_assigned);
        ranges[1] = ScanRange::get(from, from_assigned);
        ranges[2] = ScanRange::get(edge, edge_assigned);
        undirected = std::make_unique<IndexScan<3>>(
            *gql_model.u_edge,
            std::move(ranges)
        );
    } else {
        ranges[0] = ScanRange::get(from, from_assigned);
        ranges[1] = ScanRange::get(to, to_assigned);
        ranges[2] = ScanRange::get(edge, edge_assigned);
        undirected = std::make_unique<IndexScan<3>>(
            *gql_model.u_edge,
            std::move(ranges)
        );
    }
    undirected = std::make_unique<EdgeDirectionUndirected>(std::move(undirected), direction_var);

    std::unique_ptr<BindingIter> directed;
    if (to_assigned) {
        ranges[0] = ScanRange::get(to, to_assigned);
        ranges[1] = ScanRange::get(from, from_assigned);
        ranges[2] = ScanRange::get(edge, edge_assigned);
        directed = std::make_unique<IndexScan<3>>(*gql_model.to_from_edge, std::move(ranges));
    } else {
        ranges[0] = ScanRange::get(from, from_assigned);
        ranges[1] = ScanRange::get(to, to_assigned);
        ranges[2] = ScanRange::get(edge, edge_assigned);
        directed = std::make_unique<IndexScan<3>>(*gql_model.from_to_edge, std::move(ranges));
    }
    directed = add_direction_iter(std::move(directed));

    std::vector<std::unique_ptr<BindingIter>> iters;
    iters.push_back(std::move(directed));
    iters.push_back(std::move(undirected));
    return std::make_unique<Union>(std::move(iters));
}

std::unique_ptr<BindingIter> UndirectedDirectedEdgePlan::add_direction_iter(std::unique_ptr<BindingIter> iter
) const
{
    if (direction.id == ObjectId::DIRECTION_RIGHT) {
        return std::make_unique<EdgeDirectionRight>(std::move(iter), direction_var);
    } else if (direction.id == ObjectId::DIRECTION_LEFT) {
        return std::make_unique<EdgeDirectionLeft>(std::move(iter), direction_var);
    }
    throw LogicException("UndirectedDirectedEdgePlan must have a valid direction.");
}

bool UndirectedDirectedEdgePlan::get_leapfrog_iter(
    std::vector<std::unique_ptr<LeapfrogIter>>& /* leapfrog_iters */,
    std::vector<VarId>& /* var_order */,
    uint_fast32_t& /* enumeration_level */
) const
{
    return false;
}

void UndirectedDirectedEdgePlan::print(std::ostream& os, int indent) const
{
    os << std::string(indent, ' ');
    os << "UndirectedDirectedEdge(";
    os << "from: " << from;
    os << ", to: " << to;
    os << ", edge: " << edge;
    os << ")";
}
