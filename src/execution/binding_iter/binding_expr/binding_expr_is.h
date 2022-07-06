#pragma once

#include <memory>

#include "parser/query/expr/bool_expr/expr_comaprision/expr_is.h"
#include "execution/binding_iter/binding_expr/binding_expr.h"

class BindingExprIs : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    bool negation;

    ExprTypeName type;

    BindingExprIs(std::unique_ptr<BindingExpr> expr, bool negation, ExprTypeName type) :
        expr     (std::move(expr)),
        negation (negation),
        type     (type) { }

    GraphObject eval(const BindingIter& binding_iter) const override {
        auto eval = expr->eval(binding_iter);
        switch (type) {
            case ExprTypeName::NULL_:
                if (eval.type == GraphObjectType::NULL_OBJ) {
                    return GraphObject::make_bool(true ^ negation);
                } else {
                    return GraphObject::make_bool(false ^ negation);
                }

            case ExprTypeName::INTEGER:
                if (eval.type == GraphObjectType::INT) {
                    return GraphObject::make_bool(true ^ negation);
                } else {
                    return GraphObject::make_bool(false ^ negation);
                }

            case ExprTypeName::FLOAT:
                if (eval.type == GraphObjectType::FLOAT) {
                    return GraphObject::make_bool(true ^ negation);
                } else {
                    return GraphObject::make_bool(false ^ negation);
                }

            case ExprTypeName::BOOL:
                if (eval.type == GraphObjectType::BOOL) {
                    return GraphObject::make_bool(true ^ negation);
                } else {
                    return GraphObject::make_bool(false ^ negation);
                }

            case ExprTypeName::STRING:
                if (eval.type == GraphObjectType::STR_INLINED
                    || eval.type == GraphObjectType::STR_EXTERNAL)
                {
                    return GraphObject::make_bool(true ^ negation);
                } else {
                    return GraphObject::make_bool(false ^ negation);
                }
        }
        return GraphObject::make_bool(false ^ negation);
    }
};
