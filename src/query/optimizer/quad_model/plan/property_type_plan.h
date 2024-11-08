#pragma once

#include "query/optimizer/plan/plan.h"
#include "query/query_context.h"

class PropertyTypePlan : public Plan {
public:
    PropertyTypePlan(VarId object, ObjectId key, VarId value, uint64_t types_bitmap);

    PropertyTypePlan(const PropertyTypePlan& other) :
        object(other.object),
        key(other.key),
        value(other.value),
        types_bitmap(other.types_bitmap),
        object_assigned(other.object_assigned)
    { }

    std::unique_ptr<Plan> clone() const override
    {
        return std::make_unique<PropertyTypePlan>(*this);
    }

    int relation_size() const override
    {
        return 3;
    }

    double estimate_cost() const override;
    double estimate_output_size() const override;

    std::set<VarId> get_vars() const override;
    void set_input_vars(const std::set<VarId>& input_vars) override;

    std::unique_ptr<BindingIter> get_binding_iter() const override;

    bool get_leapfrog_iter(
        std::vector<std::unique_ptr<LeapfrogIter>>& leapfrog_iters,
        std::vector<VarId>& var_order,
        uint_fast32_t& enumeration_level
    ) const override;

    void print(std::ostream& os, int indent) const override;
    std::string printTypesFromBitmap(uint64_t types_bitmap) const;

private:
    VarId object;
    ObjectId key;
    VarId value;
    uint64_t types_bitmap;

    bool object_assigned;
};
