#pragma once

#include <memory>

#include "execution/binding_iter/binding_expr/binding_expr.h"

class BindingExprOr: public BindingExpr {
public:
    std::vector<std::unique_ptr<BindingExpr>> or_list;

    BindingExprOr(std::vector<std::unique_ptr<BindingExpr>>&& or_list) :
        or_list (std::move(or_list)) { }

    GraphObject eval(const BindingIter& binding_iter) const override {
        for (auto& expr : or_list) {
            auto eval = expr->eval(binding_iter);
            if (eval == GraphObject::make_bool(true)) {
                return GraphObject::make_bool(true);
            } else if (eval == GraphObject::make_bool(false)) {
                continue;
            } else {
                return GraphObject::make_null();
            }
        }
        return GraphObject::make_bool(false);
    }
};
