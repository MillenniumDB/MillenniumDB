#pragma once

#include <memory>

#include "execution/binding_iter/binding_expr/binding_expr.h"

class BindingExprSubtraction : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> lhs;
    std::unique_ptr<BindingExpr> rhs;

    BindingExprSubtraction(std::unique_ptr<BindingExpr> lhs, std::unique_ptr<BindingExpr> rhs) :
        lhs (std::move(lhs)),
        rhs (std::move(rhs)) { }

    GraphObject eval(const BindingIter& binding_iter) const override {
        return lhs->eval(binding_iter) - rhs->eval(binding_iter);
    }
};
