#include "directed_left_right_edge_plan.h"

#include "graph_models/gql/gql_model.h"
#include "query/executor/binding_iter/edge_direction_left.h"
#include "query/executor/binding_iter/edge_direction_right.h"
#include "query/executor/binding_iter/edge_table_lookup_gql.h"
#include "query/executor/binding_iter/index_scan.h"
#include "query/executor/binding_iter/union.h"

using namespace GQL;

DirectedLeftRightEdgePlan::DirectedLeftRightEdgePlan(Id edge, Id from, Id to, VarId direction_var) :
    edge(edge),
    from(from),
    to(to),
    direction_var(direction_var),
    edge_assigned(edge.is_OID()),
    from_assigned(from.is_OID()),
    to_assigned(to.is_OID())
{ }

double DirectedLeftRightEdgePlan::estimate_cost() const
{
    return estimate_output_size();
}

double DirectedLeftRightEdgePlan::estimate_output_size() const
{
    return gql_model.catalog.directed_edges_count;
}

std::set<VarId> DirectedLeftRightEdgePlan::get_vars() const
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

void DirectedLeftRightEdgePlan::set_input_vars(const std::set<VarId>& input_vars)
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

std::unique_ptr<BindingIter> DirectedLeftRightEdgePlan::get_binding_iter() const
{
    if (edge_assigned) {
        if (from == to) {
            std::unique_ptr<BindingIter> iter = std::make_unique<EdgeTableLookupGQL>(
                *gql_model.directed_edges,
                edge.get_var(),
                from,
                to,
                from_assigned,
                to_assigned,
                ObjectId::MASK_DIRECTED_EDGE
            );
            return std::make_unique<EdgeDirectionRight>(
                std::move(iter),
                direction_var
            );
        }
        std::unique_ptr<BindingIter> directed_right = std::make_unique<EdgeTableLookupGQL>(
            *gql_model.directed_edges,
            edge.get_var(),
            from,
            to,
            from_assigned,
            to_assigned,
            ObjectId::MASK_DIRECTED_EDGE
        );
        directed_right = std::make_unique<EdgeDirectionRight>(std::move(directed_right), direction_var);

        std::unique_ptr<BindingIter> directed_left = std::make_unique<EdgeTableLookupGQL>(
            *gql_model.directed_edges,
            edge.get_var(),
            to,
            from,
            to_assigned,
            from_assigned,
            ObjectId::MASK_DIRECTED_EDGE
        );
        directed_left = std::make_unique<EdgeDirectionLeft>(std::move(directed_left), direction_var);

        std::vector<std::unique_ptr<BindingIter>> iters;
        iters.push_back(std::move(directed_left));
        iters.push_back(std::move(directed_right));
        return std::make_unique<Union>(std::move(iters));
    }

    if (from == to) {
        std::array<std::unique_ptr<ScanRange>, 2> ranges;
        ranges[0] = ScanRange::get(from, from_assigned);
        ranges[1] = ScanRange::get(edge, edge_assigned);
        auto iter = std::make_unique<IndexScan<2>>(*gql_model.equal_d_edge, std::move(ranges));
        return std::make_unique<EdgeDirectionRight>(
            std::move(iter),
            direction_var
        );
    }

    std::array<std::unique_ptr<ScanRange>, 3> ranges_right;
    std::array<std::unique_ptr<ScanRange>, 3> ranges_left;
    std::unique_ptr<BindingIter> directed_right;
    std::unique_ptr<BindingIter> directed_left;

    if (to_assigned) {
        ranges_right[0] = ScanRange::get(to, to_assigned);
        ranges_right[1] = ScanRange::get(from, from_assigned);
        ranges_right[2] = ScanRange::get(edge, edge_assigned);

        ranges_left[0] = ScanRange::get(to, to_assigned);
        ranges_left[1] = ScanRange::get(from, from_assigned);
        ranges_left[2] = ScanRange::get(edge, edge_assigned);

        directed_left = std::make_unique<EdgeDirectionLeft>(
            std::make_unique<IndexScan<3>>(*gql_model.from_to_edge, std::move(ranges_left)),
            direction_var
        );
        directed_right = std::make_unique<EdgeDirectionRight>(
            std::make_unique<IndexScan<3>>(*gql_model.to_from_edge, std::move(ranges_right)),
            direction_var
        );
    } else {
        ranges_right[0] = ScanRange::get(from, from_assigned);
        ranges_right[1] = ScanRange::get(to, to_assigned);
        ranges_right[2] = ScanRange::get(edge, edge_assigned);

        ranges_left[0] = ScanRange::get(from, from_assigned);
        ranges_left[1] = ScanRange::get(to, to_assigned);
        ranges_left[2] = ScanRange::get(edge, edge_assigned);

        directed_right = std::make_unique<EdgeDirectionRight>(
            std::make_unique<IndexScan<3>>(*gql_model.from_to_edge, std::move(ranges_right)),
            direction_var
        );
        directed_left = std::make_unique<EdgeDirectionLeft>(
            std::make_unique<IndexScan<3>>(*gql_model.to_from_edge, std::move(ranges_left)),
            direction_var
        );
    }

    std::vector<std::unique_ptr<BindingIter>> iters;
    iters.push_back(std::move(directed_right));
    iters.push_back(std::move(directed_left));
    return std::make_unique<Union>(std::move(iters));
}

bool DirectedLeftRightEdgePlan::get_leapfrog_iter(
    std::vector<std::unique_ptr<LeapfrogIter>>& /* leapfrog_iters */,
    std::vector<VarId>& /* var_order */,
    uint_fast32_t& /* enumeration_level */
) const
{
    return false;
}

void DirectedLeftRightEdgePlan::print(std::ostream& os, int indent) const
{
    os << std::string(indent, ' ');
    os << "DirectedLeftRightEdge(";
    os << "from: " << from;
    os << ", to: " << to;
    os << ", edge: " << edge;
    os << ")";
}
