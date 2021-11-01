#ifndef BASE__CONSTANT_CONDITION_H_
#define BASE__CONSTANT_CONDITION_H_

#include "base/graph/condition/condition.h"

class ConstantCondition : public Condition {
public:
    const bool constant;

    ConstantCondition (bool constant) : constant(constant) { }

    bool eval(Binding&) const {
        return constant;
    }

    ConditionType type() const {
        return ConditionType::constant;
    }
};

#endif // BASE__CONSTANT_CONDITION_H_
