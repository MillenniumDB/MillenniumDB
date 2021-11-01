#ifndef BASE__EQUALS_H_
#define BASE__EQUALS_H_

#include "base/graph/condition/comparisons/comparison.h"

class Equals : public Comparison {
public:
    Equals(std::unique_ptr<ValueAssign> lhs, std::unique_ptr<ValueAssign> rhs) :
        Comparison(std::move(lhs), std::move(rhs)) { }

    bool compare(const GraphObject lhs, const GraphObject rhs) const override {
        return lhs == rhs;
    }
};

#endif // BASE__EQUALS_H_
