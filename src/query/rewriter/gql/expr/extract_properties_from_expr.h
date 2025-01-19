#pragma once

#include "query/parser/expr/expr.h"
#include "query/parser/expr/expr_visitor.h"
#include "query/parser/expr/gql_exprs.h"
#include "query/parser/op/gql/graph_pattern/op_property.h"

using namespace GQL;

class ExtractPropertiesFromExpr : public ExprVisitor {
public:
    std::vector<PropertyValue> properties;

    std::unique_ptr<Expr> tmp;
    bool key_setted = false;
    bool value_setted = false;

    VarId current_object;
    ObjectId current_key;
    VarId current_var_with_property;
    ObjectId current_value;
    VarType::Type current_type;

    ExtractPropertiesFromExpr() :
        current_object(0),
        current_var_with_property(0)
    { }

    void reset_key_value()
    {
        key_setted = false;
        value_setted = false;
    }

    void clear_properties()
    {
        properties.clear();
    }

    void visit(ExprAnd& and_expr)
    {
        std::vector<std::unique_ptr<Expr>> and_exprs;
        for (auto& expr : and_expr.exprs) {
            expr->accept_visitor(*this);

            if (tmp != nullptr) {
                and_exprs.push_back(std::move(tmp));
            }
        }

        if (and_exprs.size() > 1) {
            tmp = std::make_unique<ExprAnd>(std::move(and_exprs));
        } else if (and_exprs.size() == 1) {
            tmp = std::move(and_exprs[0]);
        }
    }

    void visit(ExprEquals& expr_equals)
    {
        reset_key_value();
        expr_equals.lhs->accept_visitor(*this);
        auto lhs = std::move(tmp);
        expr_equals.rhs->accept_visitor(*this);
        auto rhs = std::move(tmp);

        if (key_setted && value_setted) {
            properties.push_back(
                { current_object, current_key, current_var_with_property, current_value, current_type }
            );
            tmp = nullptr;
        } else {
            tmp = std::make_unique<ExprEquals>(std::move(lhs), std::move(rhs));
        }
    }

    void visit(ExprProperty& expr_property)
    {
        current_object = expr_property.object;
        current_key = expr_property.key;
        current_var_with_property = expr_property.value;
        current_type = expr_property.type;
        key_setted = true;
        tmp = expr_property.clone();
    }

    void visit(ExprTerm& expr_term)
    {
        current_value = expr_term.term;
        value_setted = true;
        tmp = expr_term.clone();
    }

    void visit(ExprOr& expr_or)
    {
        tmp = expr_or.clone();
    }

    void visit(ExprHasNodeLabel& expr)
    {
        tmp = expr.clone();
    }

    void visit(ExprHasEdgeLabel& expr)
    {
        tmp = expr.clone();
    }

    void visit(ExprVar& expr)
    {
        tmp = expr.clone();
    }

    void visit(ExprXor& expr)
    {
        tmp = expr.clone();
    }

    void visit(ExprConcat& expr)
    {
        tmp = expr.clone();
    }

    void visit(ExprGreaterOrEquals& expr)
    {
        tmp = expr.clone();
    }

    void visit(ExprGreater& expr)
    {
        tmp = expr.clone();
    }

    void visit(ExprIs& expr)
    {
        tmp = expr.clone();
    }

    void visit(ExprLessOrEquals& expr)
    {
        tmp = expr.clone();
    }

    void visit(ExprLess& expr)
    {
        tmp = expr.clone();
    }

    void visit(ExprNotEquals& expr)
    {
        tmp = expr.clone();
    }

    void visit(ExprAddition& expr)
    {
        tmp = expr.clone();
    }

    void visit(ExprSubtraction& expr)
    {
        tmp = expr.clone();
    }

    void visit(ExprMultiplication& expr)
    {
        tmp = expr.clone();
    }

    void visit(ExprDivision& expr)
    {
        tmp = expr.clone();
    }

    void visit(ExprNot& expr)
    {
        tmp = expr.clone();
    }
};
