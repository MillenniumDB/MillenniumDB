#ifndef BASE__DISJUNCTION_H_
#define BASE__DISJUNCTION_H_

#include <memory>
#include <vector>

#include "base/graph/condition/condition.h"

class Disjunction : public Condition {
public:
    std::vector<std::unique_ptr<Condition>> conditions;

    Disjunction() { }
    Disjunction(std::vector<std::unique_ptr<Condition>> conditions)
        : conditions (std::move(conditions)) { }

    bool eval(Binding& binding) const {
        for (auto& condition : conditions) {
            if (condition->eval(binding)) {
                return true;
            }
        }
        return false;
    }

    ConditionType type() const {
        return ConditionType::disjunction;
    }

    void add(std::unique_ptr<Condition> condition) {
        conditions.push_back(std::move(condition));
    }
};

#endif // BASE__DISJUNCTION_H_
