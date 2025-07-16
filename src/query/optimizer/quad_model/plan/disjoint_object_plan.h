#pragma once

#include "query/optimizer/plan/plan.h"

class DisjointObjectPlan : public Plan {
public:
    DisjointObjectPlan(const VarId object_var) :
        object_var (object_var) { }

    DisjointObjectPlan(const DisjointObjectPlan& other) :
        object_var (other.object_var) { }

    std::unique_ptr<Plan> clone() const override {
        return std::make_unique<DisjointObjectPlan>(*this);
    }

    int relation_size() const override { return 1; }

    double estimate_cost() const override;
    double estimate_output_size() const override;

    std::set<VarId> get_vars() const override;
    void set_input_vars(const std::set<VarId>& input_vars) override;

    std::unique_ptr<BindingIter> get_binding_iter() const override;

    bool get_leapfrog_iter(std::vector<std::unique_ptr<LeapfrogIter>>&,
                           std::vector<VarId>&,
                           uint_fast32_t&) const override { return false; }

    void print(std::ostream& os, int indent) const override;

private:
    const VarId object_var;
};
