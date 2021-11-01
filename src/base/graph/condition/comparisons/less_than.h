#ifndef BASE__LESS_THAN_H_
#define BASE__LESS_THAN_H_

#include "base/graph/condition/comparisons/comparison.h"

class LessThan : public Comparison {
public:
    LessThan(std::unique_ptr<ValueAssign> lhs, std::unique_ptr<ValueAssign> rhs) :
        Comparison(std::move(lhs), std::move(rhs)) { }

    bool compare(const GraphObject lhs, const GraphObject rhs) const override {
        return lhs < rhs;
    }
};

#endif // BASE__LESS_THAN_H_
