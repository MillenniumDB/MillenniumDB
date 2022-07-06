#pragma once

#include <memory>

#include "execution/binding_iter/binding_expr/binding_expr.h"

class BindingExprUnaryPlus : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    BindingExprUnaryPlus(std::unique_ptr<BindingExpr> expr) :
        expr (std::move(expr)) { }

    GraphObject eval(const BindingIter& binding_iter) const override {
        return expr->eval(binding_iter) * GraphObject::make_int(1);
    }
};
