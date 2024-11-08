#pragma once

#include <memory>
#include <vector>

#include "query/executor/binding_iter.h"
#include "query/optimizer/plan/plan.h"

/*
To choose the variable order we have 2 approaches depending
on the presence of assigned vars.

- When no assigned vars are present:
For each join variable we make a leapfrog join with all relations
that contains that variable, and we do seeks on the first level until
we end or reach a certain constant amount of seeks. With that we estimate
how many seeks we would need to finish the join on the first level.

- When assigned vars are present:
1. Assign a cost to each variable, equal to the min cost of the relations the variable appears in.
   The number of appearances is used as a tiebreaker.

2. Choose variables one by one according to their lowest cost, considering only those that are connected
   to a variable already chosen (If no variable has been chosen yet or there is no variable connected to
   a variable already chosen, consider all variables)
*/
class LeapfrogOptimizer {
public:
    // may return nullptr if leapfrog is not possible
    static std::unique_ptr<BindingIter> try_get_iter_with_assigned(
        const std::vector<std::unique_ptr<Plan>>& base_plans
    );

    static std::unique_ptr<BindingIter> try_get_iter_without_assigned(
        const std::vector<std::unique_ptr<Plan>>& base_plans
    );
};
