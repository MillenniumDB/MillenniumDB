#ifndef BASE__VALUE_ASSIGN_H_
#define BASE__VALUE_ASSIGN_H_

#include <memory>

#include "base/binding/binding.h"
#include "base/graph/graph_object.h"


// Abstract class
class ValueAssign {
public:
    virtual ~ValueAssign() = default;
    virtual GraphObject get_value(Binding&) const = 0;
};

#endif // BASE__VALUE_ASSIGN_H_
