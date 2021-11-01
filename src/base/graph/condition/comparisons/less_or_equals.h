#ifndef BASE__LESS_OR_EQUALS_H_
#define BASE__LESS_OR_EQUALS_H_

#include "base/graph/condition/comparisons/comparison.h"

class LessOrEquals : public Comparison {
public:
    LessOrEquals(std::unique_ptr<ValueAssign> lhs, std::unique_ptr<ValueAssign> rhs) :
        Comparison(std::move(lhs), std::move(rhs)) { }

    bool compare(const GraphObject lhs, const GraphObject rhs) const override {
        return lhs <= rhs;
    }
};

#endif // BASE__LESS_OR_EQUALS_H_
