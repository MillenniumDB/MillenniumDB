#pragma once

#include "base/binding/binding_iter.h"
#include "base/graph_object/graph_object.h"

// Abstract class
class BindingExpr {
public:
    virtual ~BindingExpr() = default;

    virtual GraphObject eval(const BindingIter&) const = 0;
};
