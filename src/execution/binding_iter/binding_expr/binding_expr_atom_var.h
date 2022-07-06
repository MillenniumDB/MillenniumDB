#pragma once

#include <memory>

#include "base/ids/var_id.h"
#include "execution/binding_iter/binding_expr/binding_expr.h"

class BindingExprAtomVar : public BindingExpr {
public:
    VarId var_id;

    BindingExprAtomVar(VarId var_id) :
        var_id (var_id) { }

    GraphObject eval(const BindingIter& binding_iter) const override {
        return binding_iter[var_id];
    }
};

