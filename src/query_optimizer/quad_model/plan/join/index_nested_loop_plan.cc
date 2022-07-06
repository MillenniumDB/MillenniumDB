#include "index_nested_loop_plan.h"

#include "base/exceptions.h"
#include "execution/binding_id_iter/index_nested_loop_join.h"

using namespace std;

IndexNestedLoopPlan::IndexNestedLoopPlan(unique_ptr<Plan> _lhs, unique_ptr<Plan> _rhs) :
    lhs(move(_lhs)), rhs(move(_rhs))
{
    rhs->set_input_vars(lhs->get_vars());

    const auto lhs_output_size = lhs->estimate_output_size();
    estimated_output_size = lhs_output_size * rhs->estimate_output_size();

    if (lhs_output_size > 1) {
        estimated_cost = lhs->estimate_cost() + (lhs_output_size * rhs->estimate_cost());
    } else {
        estimated_cost = lhs->estimate_cost() + (rhs->estimate_cost());
    }
}


void IndexNestedLoopPlan::print(std::ostream& os, int indent, const std::vector<std::string>& var_names) const {
    for (int i = 0; i < indent; ++i) {
        os << ' ';
    }
    os << "IndexNestedLoopJoin(\n";
    lhs->print(os, indent + 2, var_names);
    os << ",\n";
    rhs->print(os, indent + 2, var_names);
    os << "\n";
    for (int i = 0; i < indent; ++i) {
        os << ' ';
    }
    os << ")";
}

std::set<VarId> IndexNestedLoopPlan::get_vars() const {
    auto result = lhs->get_vars();
    for (auto var : rhs->get_vars()) {
        result.insert(var);
    }
    return result;
}


void IndexNestedLoopPlan::set_input_vars(const std::set<VarId>& /*input_vars*/) {
    throw LogicException("NestedLoop only works for left deep plans.");
}


unique_ptr<BindingIdIter> IndexNestedLoopPlan::get_binding_id_iter(ThreadInfo* thread_info) const {
    return make_unique<IndexNestedLoopJoin>(lhs->get_binding_id_iter(thread_info),
                                            rhs->get_binding_id_iter(thread_info));
}
