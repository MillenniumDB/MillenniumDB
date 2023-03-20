#pragma once

#include <memory>

#include "parser/query/expr/bool_expr/expr_comaprision/expr_is.h"
#include "execution/binding_iter/binding_expr/binding_expr.h"
#include "execution/graph_object/graph_object_types.h"

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
                    return GraphObjectFactory::make_bool(true ^ negation);
                } else {
                    return GraphObjectFactory::make_bool(false ^ negation);
                }

            case ExprTypeName::INTEGER:
                if (eval.type == GraphObjectType::INT) {
                    return GraphObjectFactory::make_bool(true ^ negation);
                } else {
                    return GraphObjectFactory::make_bool(false ^ negation);
                }

            case ExprTypeName::FLOAT:
                if (eval.type == GraphObjectType::FLOAT) {
                    return GraphObjectFactory::make_bool(true ^ negation);
                } else {
                    return GraphObjectFactory::make_bool(false ^ negation);
                }

            case ExprTypeName::BOOL:
                if (eval.type == GraphObjectType::BOOL) {
                    return GraphObjectFactory::make_bool(true ^ negation);
                } else {
                    return GraphObjectFactory::make_bool(false ^ negation);
                }

            case ExprTypeName::STRING:
                if (eval.type == GraphObjectType::STR_INLINED
                    || eval.type == GraphObjectType::STR_EXTERNAL)
                {
                    return GraphObjectFactory::make_bool(true ^ negation);
                } else {
                    return GraphObjectFactory::make_bool(false ^ negation);
                }
        }
        return GraphObjectFactory::make_bool(false ^ negation);
    }
};
