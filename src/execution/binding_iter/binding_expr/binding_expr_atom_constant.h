#pragma once

#include <memory>
#include <string>

#include "base/ids/var_id.h"
#include "execution/binding_iter/binding_expr/binding_expr.h"

class BindingExprAtomConstant : public BindingExpr {
public:
    GraphObject value;

    BindingExprAtomConstant(GraphObject value) :
        value (value) { }

    GraphObject eval(const BindingIter&) const override {
        return value;
    }
};
