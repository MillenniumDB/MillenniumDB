#pragma once

#include <memory>

#include "execution/binding_iter/binding_expr/binding_expr.h"

class BindingExprEquals : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> lhs;
    std::unique_ptr<BindingExpr> rhs;

    BindingExprEquals(std::unique_ptr<BindingExpr> lhs, std::unique_ptr<BindingExpr> rhs) :
        lhs (std::move(lhs)),
        rhs (std::move(rhs)) { }

    GraphObject eval(const BindingIter& binding_iter) const override {
        return GraphObject::make_bool(lhs->eval(binding_iter) == rhs->eval(binding_iter));
    }
};
