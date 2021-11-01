#ifndef BASE__NEGATION_H_
#define BASE__NEGATION_H_

#include <memory>

#include "base/graph/condition/condition.h"

class Negation : public Condition {
public:
    std::unique_ptr<Condition> condition;

    Negation(std::unique_ptr<Condition> condition)
        : condition(std::move(condition)) { }

    bool eval(Binding& binding) const {
        return !condition->eval(binding);
    }

    ConditionType type() const {
        return ConditionType::negation;
    }
};

#endif // BASE__NEGATION_H_
