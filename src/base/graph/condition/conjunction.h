#ifndef BASE__COJUNCTION_H_
#define BASE__COJUNCTION_H_

#include <memory>
#include <vector>

#include "base/graph/condition/condition.h"

class Conjunction : public Condition {
public:
    std::vector<std::unique_ptr<Condition>> conditions;

    Conjunction() { }
    Conjunction(std::vector<std::unique_ptr<Condition>> conditions)
        : conditions (std::move(conditions)) { }

    bool eval(Binding& binding) const {
        for (auto& condition : conditions) {
            if (!condition->eval(binding)) {
                return false;
            }
        }
        return true;
    }

    ConditionType type() const {
        return ConditionType::conjunction;
    }

    void add(std::unique_ptr<Condition> condition) {
        conditions.push_back(std::move(condition));
    }
};

#endif // BASE__COJUNCTION_H_
