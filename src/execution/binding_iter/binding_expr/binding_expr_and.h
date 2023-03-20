#pragma once

#include <memory>

#include "execution/graph_object/graph_object_factory.h"
#include "execution/binding_iter/binding_expr/binding_expr.h"

class BindingExprAnd: public BindingExpr {
public:
    std::vector<std::unique_ptr<BindingExpr>> and_list;

    BindingExprAnd(std::vector<std::unique_ptr<BindingExpr>>&& and_list) :
        and_list (std::move(and_list)) { }

    GraphObject eval(const BindingIter& binding_iter) const override {
        for (auto& expr : and_list) {
            auto eval = expr->eval(binding_iter);
            if (eval == GraphObjectFactory::make_bool(true)) {
                continue;
            } else if (eval == GraphObjectFactory::make_bool(false)) {
                return GraphObjectFactory::make_bool(false);
            } else {
                return GraphObjectFactory::make_null();
            }
        }
        return GraphObjectFactory::make_bool(true);
    }
};
