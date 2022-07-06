#pragma once

#include <memory>

#include "execution/binding_iter/binding_expr/binding_expr.h"

class BindingExprAnd: public BindingExpr {
public:
    std::vector<std::unique_ptr<BindingExpr>> and_list;

    BindingExprAnd(std::vector<std::unique_ptr<BindingExpr>>&& and_list) :
        and_list (std::move(and_list)) { }

    GraphObject eval(const BindingIter& binding_iter) const override {
        for (auto& expr : and_list) {
            auto eval = expr->eval(binding_iter);
            if (eval == GraphObject::make_bool(true)) {
                continue;
            } else if (eval == GraphObject::make_bool(false)) {
                return GraphObject::make_bool(false);
            } else {
                return GraphObject::make_null();
            }
        }
        return GraphObject::make_bool(true);
    }
};
