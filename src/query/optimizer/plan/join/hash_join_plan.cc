#include "hash_join_plan.h"

#include "query/exceptions.h"
#include "query/executor/binding_iter/hash_join/hash_join_grace.h"
#include "query/executor/binding_iter/hash_join/hash_join_in_buffer.h"
#include "query/executor/binding_iter/hash_join/hash_join_in_memory.h"

using HashJoin = HashJoinGrace;
// using HashJoin = HashJoinInMemory;
// using HashJoin = HashJoinInBuffer;

HashJoinPlan::HashJoinPlan(
    std::unique_ptr<Plan> _lhs,
    std::unique_ptr<Plan> _rhs
) :
    lhs  (std::move(_lhs)),
    rhs  (std::move(_rhs))
{
    estimated_output_size = lhs->estimate_output_size() * rhs->estimate_output_size();
    estimated_cost = lhs->estimate_cost() + rhs->estimate_cost();
}


void HashJoinPlan::print(std::ostream& os, int indent) const {
    for (int i = 0; i < indent; ++i) {
        os << ' ';
    }
    os << "HashJoin(\n";
    lhs->print(os, indent + 2);
    os << ",\n";
    rhs->print(os, indent + 2);
    os << "\n";
    for (int i = 0; i < indent; ++i) {
        os << ' ';
    }
    os << ")";
}


std::set<VarId> HashJoinPlan::get_vars() const {
    auto result = lhs->get_vars();
    for (auto var : rhs->get_vars()) {
        result.insert(var);
    }
    return result;
}


void HashJoinPlan::set_input_vars(const std::set<VarId>& /*input_vars*/) {
    throw LogicException("HashJoin only works for left deep plans.");
}


std::unique_ptr<BindingIter> HashJoinPlan::get_binding_iter() const {
    std::vector<VarId> common_vars;
    std::vector<VarId> only_left_vars;
    std::vector<VarId> only_right_vars;

    const auto lhs_vars = lhs->get_vars();
    const auto rhs_vars = rhs->get_vars();

    for (auto left_var : lhs_vars) {
        if (rhs_vars.find(left_var) == rhs_vars.end()) {
            only_left_vars.push_back(left_var);
        } else {
            common_vars.push_back(left_var);
        }
    }
    for (auto right_var : rhs_vars) {
        if (lhs_vars.find(right_var) != lhs_vars.end()){
            only_right_vars.push_back(right_var);
        }
    }

    return std::make_unique<HashJoin>(
        lhs->get_binding_iter(),
        rhs->get_binding_iter(),
        std::move(only_left_vars),
        std::move(common_vars),
        std::move(only_right_vars));
}
