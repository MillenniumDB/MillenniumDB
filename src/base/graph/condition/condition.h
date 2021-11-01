#ifndef BASE__CONDITION_H_
#define BASE__CONDITION_H_

#include "base/binding/binding.h"
#include "base/graph/graph_object.h"

enum class ConditionType {
    comparison,
    conjunction,
    constant,
    disjunction,
    negation
};

// Abstract class
class Condition {
public:
    virtual ~Condition() = default;
    virtual bool eval(Binding&) const = 0;
    virtual ConditionType type() const = 0;
};

#endif // BASE__CONDITION_H_
