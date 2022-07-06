#pragma once

#include <memory>
#include <vector>

#include "base/binding/binding_id_iter.h"
#include "base/ids/var_id.h"
#include "storage/index/leapfrog/leapfrog_iter.h"

/* LeapfrogJoin is a Worst-Case Optimal join algorithm
 * that performs a join between an arbitrary number of relations
 * instead of the typical binary join.
 */
class LeapfrogJoin : public BindingIdIter {
public:
    LeapfrogJoin(std::vector<std::unique_ptr<LeapfrogIter>> leapfrog_iters,
                 std::vector<VarId> var_order,
                 int_fast32_t enumeration_level);

    void analyze(std::ostream& os, int indent = 0) const override;
    void begin(BindingId& parent_binding) override;
    bool next() override;
    void reset() override;
    void assign_nulls() override;

private:
    std::vector<std::unique_ptr<LeapfrogIter>> leapfrog_iters;

    // At first it contains variables from intersection_vars
    // then it contains variables from enumeration_vars
    std::vector<VarId> var_order;

    BindingId* parent_binding;

    const int_fast32_t enumeration_level;

    // level will vary between [0, enumeration_level]
    // when level=-1 means there is no more tuples left
    int_fast32_t level;

    // iters_for_var[i] is a list of (not-null) pointers of iterators for the variable at var_order[base_level+i].
    std::vector<std::vector<LeapfrogIter*>> iters_for_var;

    uint_fast32_t results_found = 0;
    uint_fast32_t seeks = 0;

    void up();
    void down();
    bool find_intersection_for_current_level();
};
