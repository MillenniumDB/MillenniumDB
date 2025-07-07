#include "any_direction_edge_plan.h"

#include "graph_models/gql/gql_model.h"
#include "query/executor/binding_iter/edge_direction_left.h"
#include "query/executor/binding_iter/edge_direction_right.h"
#include "query/executor/binding_iter/edge_direction_undirected.h"
#include "query/executor/binding_iter/edge_table_lookup_gql.h"
#include "query/executor/binding_iter/index_scan.h"
#include "query/executor/binding_iter/union.h"

using namespace GQL;

AnyDirectionEdgePlan::AnyDirectionEdgePlan(Id edge, Id from, Id to, VarId direction_var) :
    edge(edge),
    from(from),
    to(to),
    direction_var(direction_var),
    edge_assigned(edge.is_OID()),
    from_assigned(from.is_OID()),
    to_assigned(to.is_OID())
{ }

double AnyDirectionEdgePlan::estimate_cost() const
{
    return estimate_output_size();
}

double AnyDirectionEdgePlan::estimate_output_size() const
{
    return gql_model.catalog.directed_edges_count + gql_model.catalog.undirected_edges_count;
}

std::set<VarId> AnyDirectionEdgePlan::get_vars() const
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

void AnyDirectionEdgePlan::set_input_vars(const std::set<VarId>& input_vars)
{
    set_input_var(input_vars, from, &from_assigned);
    set_input_var(input_vars, to, &to_assigned);
    set_input_var(input_vars, edge, &edge_assigned);
}

std::unique_ptr<BindingIter> AnyDirectionEdgePlan::get_binding_iter() const
{
    if (edge_assigned) {
        auto right = std::make_unique<EdgeDirectionRight>(
            std::make_unique<EdgeTableLookupGQL>(
                *gql_model.directed_edges,
                edge.get_var(),
                from,
                to,
                from_assigned,
                to_assigned,
                ObjectId::MASK_DIRECTED_EDGE
            ),
            direction_var
        );
        auto left = std::make_unique<EdgeDirectionLeft>(
            std::make_unique<EdgeTableLookupGQL>(
                *gql_model.directed_edges,
                edge.get_var(),
                to,
                from,
                to_assigned,
                from_assigned,
                ObjectId::MASK_DIRECTED_EDGE
            ),
            direction_var
        );
        auto undirected = std::make_unique<EdgeDirectionUndirected>(
            std::make_unique<EdgeTableLookupGQL>(
                *gql_model.undirected_edges,
                edge.get_var(),
                from,
                to,
                from_assigned,
                to_assigned,
                ObjectId::MASK_UNDIRECTED_EDGE
            ),
            direction_var
        );

        if (from == to) {
            std::vector<std::unique_ptr<BindingIter>> iters;
            iters.push_back(std::move(right));
            iters.push_back(std::move(undirected));
            return std::make_unique<Union>(std::move(iters));
        }

        std::vector<std::unique_ptr<BindingIter>> iters;
        iters.push_back(std::move(right));
        iters.push_back(std::move(left));
        iters.push_back(std::move(undirected));
        return std::make_unique<Union>(std::move(iters));
    }

    if (from == to) {
        std::array<std::unique_ptr<ScanRange>, 2> ranges_undirected;
        ranges_undirected[0] = ScanRange::get(from, from_assigned);
        ranges_undirected[1] = ScanRange::get(edge, edge_assigned);
        auto undirected_iter = std::make_unique<EdgeDirectionUndirected>(
            std::make_unique<IndexScan<2>>(*gql_model.equal_u_edge, std::move(ranges_undirected)),
            direction_var
        );

        std::array<std::unique_ptr<ScanRange>, 2> ranges_directed;
        ranges_directed[0] = ScanRange::get(from, from_assigned);
        ranges_directed[1] = ScanRange::get(edge, edge_assigned);
        auto directed_iter = std::make_unique<EdgeDirectionRight>(
            std::make_unique<IndexScan<2>>(*gql_model.equal_d_edge, std::move(ranges_directed)),
            direction_var
        );

        std::vector<std::unique_ptr<BindingIter>> iters;
        iters.push_back(std::move(undirected_iter));
        iters.push_back(std::move(directed_iter));
        return std::make_unique<Union>(std::move(iters));
    }

    if (to_assigned) {
        std::array<std::unique_ptr<ScanRange>, 3> ranges_undirected;
        ranges_undirected[0] = ScanRange::get(to, to_assigned);
        ranges_undirected[1] = ScanRange::get(from, from_assigned);
        ranges_undirected[2] = ScanRange::get(edge, edge_assigned);
        auto undirected_iter = std::make_unique<EdgeDirectionUndirected>(
            std::make_unique<IndexScan<3>>(*gql_model.u_edge, std::move(ranges_undirected)),
            direction_var
        );

        std::array<std::unique_ptr<ScanRange>, 3> ranges_right;
        ranges_right[0] = ScanRange::get(to, to_assigned);
        ranges_right[1] = ScanRange::get(from, from_assigned);
        ranges_right[2] = ScanRange::get(edge, edge_assigned);
        auto right = std::make_unique<EdgeDirectionRight>(
            std::make_unique<IndexScan<3>>(*gql_model.to_from_edge, std::move(ranges_right)),
            direction_var
        );

        std::array<std::unique_ptr<ScanRange>, 3> ranges_left;
        ranges_left[0] = ScanRange::get(to, to_assigned);
        ranges_left[1] = ScanRange::get(from, from_assigned);
        ranges_left[2] = ScanRange::get(edge, edge_assigned);
        auto left = std::make_unique<EdgeDirectionLeft>(
            std::make_unique<IndexScan<3>>(*gql_model.from_to_edge, std::move(ranges_left)),
            direction_var
        );

        std::vector<std::unique_ptr<BindingIter>> iters;
        iters.push_back(std::move(undirected_iter));
        iters.push_back(std::move(left));
        iters.push_back(std::move(right));
        return std::make_unique<Union>(std::move(iters));
    }

    std::array<std::unique_ptr<ScanRange>, 3> ranges_undirected;
    ranges_undirected[0] = ScanRange::get(from, from_assigned);
    ranges_undirected[1] = ScanRange::get(to, to_assigned);
    ranges_undirected[2] = ScanRange::get(edge, edge_assigned);
    auto undirected_iter = std::make_unique<EdgeDirectionUndirected>(
        std::make_unique<IndexScan<3>>(*gql_model.u_edge, std::move(ranges_undirected)),
        direction_var
    );

    std::array<std::unique_ptr<ScanRange>, 3> ranges_right;
    ranges_right[0] = ScanRange::get(from, from_assigned);
    ranges_right[1] = ScanRange::get(to, to_assigned);
    ranges_right[2] = ScanRange::get(edge, edge_assigned);
    auto right = std::make_unique<EdgeDirectionRight>(
        std::make_unique<IndexScan<3>>(*gql_model.from_to_edge, std::move(ranges_right)),
        direction_var
    );

    std::array<std::unique_ptr<ScanRange>, 3> ranges_left;
    ranges_left[0] = ScanRange::get(from, from_assigned);
    ranges_left[1] = ScanRange::get(to, to_assigned);
    ranges_left[2] = ScanRange::get(edge, edge_assigned);
    auto left = std::make_unique<EdgeDirectionLeft>(
        std::make_unique<IndexScan<3>>(*gql_model.to_from_edge, std::move(ranges_left)),
        direction_var
    );

    std::vector<std::unique_ptr<BindingIter>> iters;
    iters.push_back(std::move(undirected_iter));
    iters.push_back(std::move(left));
    iters.push_back(std::move(right));
    return std::make_unique<Union>(std::move(iters));
}

bool AnyDirectionEdgePlan::get_leapfrog_iter(
    std::vector<std::unique_ptr<LeapfrogIter>>& /* leapfrog_iters */,
    std::vector<VarId>& /* var_order */,
    uint_fast32_t& /* enumeration_level */
) const
{
    return false;
}

void AnyDirectionEdgePlan::print(std::ostream& os, int indent) const
{
    os << std::string(indent, ' ');
    os << "AnyDirectionEdge(";
    os << "from: " << from;
    os << ", to: " << to;
    os << ", edge: " << edge;
    os << ")";
}
