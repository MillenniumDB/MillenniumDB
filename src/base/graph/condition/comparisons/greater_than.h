#ifndef BASE__GREATER_THAN_H_
#define BASE__GREATER_THAN_H_

#include "base/graph/condition/comparisons/comparison.h"

class GreaterThan : public Comparison {
public:
    GreaterThan(std::unique_ptr<ValueAssign> lhs, std::unique_ptr<ValueAssign> rhs) :
        Comparison(std::move(lhs), std::move(rhs)) { }

    bool compare(const GraphObject lhs, const GraphObject rhs) const override {
        return lhs > rhs;
    }
};

#endif // BASE__GREATER_THAN_H_
