#include "disjoint_object_plan.h"

#include "graph_models/quad_model/quad_model.h"
#include "query/executor/binding_iter/index_scan.h"
#include "query/executor/binding_iter/object_enum.h"
#include "query/executor/binding_iter/scan_ranges/unassigned_var.h"
#include "query/executor/binding_iter/union.h"

double DisjointObjectPlan::estimate_cost() const {
    return /*100.0 +*/ estimate_output_size();
}

void DisjointObjectPlan::print(std::ostream& os, int indent) const {
    for (int i = 0; i < indent; ++i) {
        os << ' ';
    }
    os << "DisjointObject(" << get_query_ctx().get_var_name(object_var) << ")";

    os << ",\n";
    for (int i = 0; i < indent; ++i) {
        os << ' ';
    }
    os << "  ↳ Estimated factor: " << estimate_output_size();
}

double DisjointObjectPlan::estimate_output_size() const {
    return quad_model.catalog.edge_count + quad_model.catalog.identifiable_nodes_count
         + quad_model.catalog.anonymous_nodes_count;
}


std::set<VarId> DisjointObjectPlan::get_vars() const {
    std::set<VarId> result;
    result.insert(object_var);
    return result;
}


void DisjointObjectPlan::set_input_vars(const std::set<VarId>& /*input_vars*/) {
    // no need to do anything
}


std::unique_ptr<BindingIter> DisjointObjectPlan::get_binding_iter() const {
    std::vector<std::unique_ptr<BindingIter>> iters;
    std::array<std::unique_ptr<ScanRange>, 1> ranges;
    ranges[0] = std::make_unique<UnassignedVar>(object_var);
    iters.push_back(std::make_unique<IndexScan<1>>(*quad_model.nodes, std::move(ranges)));

    iters.push_back(std::make_unique<ObjectEnum>(
        object_var,
        ObjectId::MASK_ANON_INLINED,
        quad_model.catalog.anonymous_nodes_count
    ));

    iters.push_back(
        std::make_unique<ObjectEnum>(object_var, ObjectId::MASK_EDGE, quad_model.catalog.edge_count)
    );
    return std::make_unique<Union>(std::move(iters));
}
