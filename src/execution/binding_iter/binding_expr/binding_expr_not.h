#pragma once

#include <memory>

#include "execution/binding_iter/binding_expr/binding_expr.h"

class BindingExprNot : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    BindingExprNot(std::unique_ptr<BindingExpr> expr) :
        expr (std::move(expr)) { }

    GraphObject eval(const BindingIter& binding_iter) const override {
        auto expr_eval = expr->eval(binding_iter);
        if (expr_eval == GraphObjectFactory::make_bool(true)) {
            return GraphObjectFactory::make_bool(false);
        } else if (expr_eval == GraphObjectFactory::make_bool(false)) {
            return GraphObjectFactory::make_bool(true);
        } else {
            return GraphObjectFactory::make_null();
        }
    }
};
