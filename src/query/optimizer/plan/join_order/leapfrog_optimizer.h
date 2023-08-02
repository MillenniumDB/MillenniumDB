#pragma once

#include <memory>
#include <vector>

#include "query/executor/binding_iter.h"
#include "query/query_context.h"
#include "query/optimizer/plan/plan.h"

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
    static std::unique_ptr<BindingIter> try_get_iter(
        const std::vector<std::unique_ptr<Plan>>& base_plans,
        const std::size_t binding_size);
};
