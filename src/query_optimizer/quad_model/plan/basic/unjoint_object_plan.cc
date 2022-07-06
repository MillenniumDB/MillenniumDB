#include "unjoint_object_plan.h"

#include "execution/binding_id_iter/object_enum.h"
#include "execution/binding_id_iter/union.h"
#include "execution/binding_id_iter/index_scan.h"
#include "query_optimizer/quad_model/quad_model.h"

using namespace std;

UnjointObjectPlan::UnjointObjectPlan(const VarId object_var_id) :
    object_var_id (object_var_id) { }


double UnjointObjectPlan::estimate_cost() const {
    return /*100.0 +*/ estimate_output_size();
}

void UnjointObjectPlan::print(std::ostream& os, int indent, const std::vector<std::string>& var_names) const {
    for (int i = 0; i < indent; ++i) {
        os << ' ';
    }
    os << "UnjointObject("  << var_names[object_var_id.id] << ")";

    os << ",\n";
    for (int i = 0; i < indent; ++i) {
        os << ' ';
    }
    os << "  ↳ Estimated factor: " << estimate_output_size();
}

double UnjointObjectPlan::estimate_output_size() const {
    return quad_model.catalog().connections_count
           + quad_model.catalog().identifiable_nodes_count
           + quad_model.catalog().anonymous_nodes_count;
}


std::set<VarId> UnjointObjectPlan::get_vars() const {
    std::set<VarId> result;
    result.insert(object_var_id);
    return result;
}


void UnjointObjectPlan::set_input_vars(const std::set<VarId>& /*input_vars*/) {
    // no need to do anything
}


unique_ptr<BindingIdIter> UnjointObjectPlan::get_binding_id_iter(ThreadInfo* thread_info) const {
    vector<unique_ptr<BindingIdIter>> iters;
    array<unique_ptr<ScanRange>, 1> ranges;
    ranges[0] = make_unique<UnassignedVar>(object_var_id);
    iters.push_back(
        make_unique<IndexScan<1>>(*quad_model.nodes,
                                  thread_info,
                                  move(ranges)));

    iters.push_back(
        make_unique<ObjectEnum>(thread_info,
                                object_var_id,
                                ObjectId::ANONYMOUS_NODE_MASK,
                                quad_model.catalog().anonymous_nodes_count));

    iters.push_back(
        make_unique<ObjectEnum>(thread_info,
                                object_var_id,
                                ObjectId::CONNECTION_MASK,
                                quad_model.catalog().connections_count));
    return make_unique<Union>(move(iters));
}
