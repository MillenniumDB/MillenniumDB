#ifndef BASE__COMPARISON_H_
#define BASE__COMPARISON_H_

#include <memory>

#include "base/binding/binding.h"
#include "base/graph/condition/condition.h"
#include "base/graph/condition/value_assign/value_assign.h"

class Comparison : public Condition {
public:
    std::unique_ptr<ValueAssign> lhs;
    std::unique_ptr<ValueAssign> rhs;

    Comparison(std::unique_ptr<ValueAssign> lhs, std::unique_ptr<ValueAssign> rhs) :
        lhs (std::move(lhs)),
        rhs (std::move(rhs)) { }

    virtual ~Comparison() = default;

    virtual bool compare(const GraphObject lhs, const GraphObject rhs) const = 0;

    inline ConditionType type() const { return ConditionType::comparison; }

    bool eval(Binding& binding) const {
        return compare(lhs->get_value(binding), rhs->get_value(binding));
    }
};

#endif // BASE__COMPARISON_H_
