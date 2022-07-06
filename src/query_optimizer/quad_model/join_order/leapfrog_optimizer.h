#pragma once

#include <memory>
#include <vector>

#include "base/binding/binding_id_iter.h"
#include "query_optimizer/quad_model/plan/plan.h"

/* To choose the variable order:
1. Assign a cost to each variable, equal to the min cost of the relations the variable appears in.
   The number of appearances is used as a tiebreaker.

2. Choose variables one by one according to their lowest cost, considering only those that are connected
   to a variable already chosen (If no variable has been chosen yet or there is no variable connected to
   a variable already chosen, consider all variables)
*/
class LeapfrogOptimizer {
public:
    // may return nullptr if leapfrog is not possible
    static std::unique_ptr<BindingIdIter> try_get_iter(
        ThreadInfo* thread_info,
        const std::vector<std::unique_ptr<Plan>>& base_plans,
        const std::vector<std::string>& var_names,
        const std::size_t binding_size);
};
