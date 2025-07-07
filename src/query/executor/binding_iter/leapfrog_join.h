#pragma once

#include <memory>
#include <vector>

#include "query/executor/binding_iter.h"
#include "query/var_id.h"
#include "storage/index/leapfrog/leapfrog_iter.h"

/* LeapfrogJoin is a Worst-Case Optimal join algorithm
 * that performs a join between an arbitrary number of relations
 * instead of the typical binary join.
 */
class LeapfrogJoin : public BindingIter {
public:
    LeapfrogJoin(
        std::vector<std::unique_ptr<LeapfrogIter>>&& leapfrog_iters,
        std::vector<VarId>&&                         var_order,
        int_fast32_t                                 enumeration_level
    ) :
        leapfrog_iters    (std::move(leapfrog_iters)),
        var_order         (std::move(var_order)),
        enumeration_level (enumeration_level),
        level             (-1) { }

    void print(std::ostream& os, int indent, bool stats) const override;

    void _begin(Binding& parent_binding) override;
    bool _next() override;
    void _reset() override;
    void assign_nulls() override;

    uint_fast32_t seeks = 0;
    std::vector<std::unique_ptr<LeapfrogIter>> leapfrog_iters;

    // At first it contains variables from intersection_vars
    // then it contains variables from enumeration_vars
    const std::vector<VarId> var_order;

    const int_fast32_t enumeration_level;

private:
    Binding* parent_binding;

    // level will vary between [0, enumeration_level]
    // when level=-1 means there is no more tuples left
    int_fast32_t level;

    // iters_for_var[i] is a list of (not-null) pointers of iterators for the variable at var_order[base_level+i].
    std::vector<std::vector<LeapfrogIter*>> iters_for_var;

    void up();
    void down();
    bool find_intersection_for_current_level();
};
