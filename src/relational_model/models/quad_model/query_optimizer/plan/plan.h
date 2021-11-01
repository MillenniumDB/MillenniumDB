#ifndef QUAD_MODEL__PLAN_H_
#define QUAD_MODEL__PLAN_H_

#include <memory>
#include <ostream>
#include <set>
#include <vector>

#include "base/binding/binding_id_iter.h"
#include "base/ids/object_id.h"
#include "base/ids/var_id.h"
#include "base/thread/thread_info.h"
#include "relational_model/models/quad_model/quad_model.h"
#include "relational_model/execution/binding_id_iter/scan_ranges/assigned_var.h"
#include "relational_model/execution/binding_id_iter/scan_ranges/term.h"
#include "relational_model/execution/binding_id_iter/scan_ranges/unassigned_var.h"
#include "storage/index/bplus_tree/leapfrog_iter.h"

// Plan is an abstract class that represents a basic relation or a join of other plans.
// The final objective of a Plan is to obtain a BindingIdIter or a LeapfrogIter
// (LeapfrogIters then are included into a LeapfrogJoin that inherits BindingIdIter)
// For a given query, many different plans are possible, so plans provide some methods
// in order to estimate which is the better plan.

class Plan {
public:
    virtual ~Plan() = default;

    virtual std::unique_ptr<Plan> duplicate() const = 0;

    virtual double estimate_cost() const = 0;

    virtual double estimate_output_size() const = 0;

    // returns a set with the variables mentioned in the relation, excluding the input vars
    virtual std::set<VarId> get_vars() const = 0;

    virtual void set_input_vars(const std::set<VarId>& input_vars) = 0;

    virtual std::unique_ptr<BindingIdIter> get_binding_id_iter(ThreadInfo* thread_info) const = 0;

    // May return nullptr if there is not an appropiate index given the local_var_order
    virtual std::unique_ptr<LeapfrogIter> get_leapfrog_iter(ThreadInfo*               thread_info,
                                                            const std::vector<VarId>& local_var_order,
                                                            uint_fast32_t             enumeration_level) const = 0;

    virtual void print(std::ostream& os, int indent, const std::vector<std::string>& var_names) const = 0;

    bool cartesian_product_needed(const Plan& other) {
        auto other_vars = other.get_vars();
        for (auto var : get_vars()) {
            if (other_vars.find(var) != other_vars.end()) {
                return false;
            }
        }
        return true;
    }

protected:
    void set_input_var(const std::set<VarId>& input_vars, Id id, bool* assigned) {
        if (std::holds_alternative<VarId>(id)) {
            auto var_id = std::get<VarId>(id);
            if (input_vars.find(var_id) != input_vars.end()) {
                *assigned = true;
            }
        }
    }
};

#endif // QUAD_MODEL__PLAN_H_
