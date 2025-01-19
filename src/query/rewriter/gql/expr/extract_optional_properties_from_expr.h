#pragma once

#include "query/parser/expr/expr.h"
#include "query/parser/expr/expr_visitor.h"
#include "query/parser/expr/gql_exprs.h"
#include "query/parser/op/gql/graph_pattern/op_property.h"

using namespace GQL;

class ExtractOptionalPropertiesFromExpr : public ExprVisitor {
public:
    std::vector<OptPropertyValue> properties;

    void visit(ExprAddition& expr_addition)
    {
        expr_addition.lhs->accept_visitor(*this);
        expr_addition.rhs->accept_visitor(*this);
    }

    void visit(ExprProperty& expr_property)
    {
        properties
            .emplace_back(expr_property.object, expr_property.key, expr_property.value, expr_property.type);
    }

    void visit(ExprOr& expr)
    {
        for (auto& expr : expr.exprs) {
            expr->accept_visitor(*this);
        }
    }

    void visit(ExprAnd& expr)
    {
        for (auto& expr : expr.exprs) {
            expr->accept_visitor(*this);
        }
    }

    void visit(ExprGreater& expr)
    {
        expr.lhs->accept_visitor(*this);
        expr.rhs->accept_visitor(*this);
    }

    void visit(ExprXor& expr)
    {
        expr.lhs->accept_visitor(*this);
        expr.rhs->accept_visitor(*this);
    }

    void visit(ExprConcat& expr)
    {
        expr.lhs->accept_visitor(*this);
        expr.rhs->accept_visitor(*this);
    }

    void visit(ExprGreaterOrEquals& expr)
    {
        expr.lhs->accept_visitor(*this);
        expr.rhs->accept_visitor(*this);
    }

    void visit(ExprLessOrEquals& expr)
    {
        expr.lhs->accept_visitor(*this);
        expr.rhs->accept_visitor(*this);
    }

    void visit(ExprLess& expr)
    {
        expr.lhs->accept_visitor(*this);
        expr.rhs->accept_visitor(*this);
    }

    void visit(ExprSubtraction& expr)
    {
        expr.lhs->accept_visitor(*this);
        expr.rhs->accept_visitor(*this);
    }

    void visit(ExprMultiplication& expr)
    {
        expr.lhs->accept_visitor(*this);
        expr.rhs->accept_visitor(*this);
    }

    void visit(ExprDivision& expr)
    {
        expr.lhs->accept_visitor(*this);
        expr.rhs->accept_visitor(*this);
    }

    void visit(ExprEquals& expr)
    {
        expr.lhs->accept_visitor(*this);
        expr.rhs->accept_visitor(*this);
    }

    void visit(ExprNotEquals& expr)
    {
        expr.lhs->accept_visitor(*this);
        expr.rhs->accept_visitor(*this);
    }

    void visit(ExprNot& expr)
    {
        expr.expr->accept_visitor(*this);
    }

    void visit(ExprTerm&) { }
    void visit(ExprHasNodeLabel&) { }
    void visit(ExprHasEdgeLabel&) { }
    void visit(ExprVar&) { }
    void visit(ExprIs&) { }
};
