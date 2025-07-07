#pragma once

#include "query/parser/expr/gql/exprs.h"
#include "query/parser/op/gql/graph_pattern/op_optional_properties.h"

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

    void visit(ExprAbs& expr)
    {
        expr.expr->accept_visitor(*this);
    }

    void visit(ExprModulo& expr)
    {
        expr.lhs->accept_visitor(*this);
        expr.rhs->accept_visitor(*this);
    }

    void visit(ExprSin& expr)
    {
        expr.expr->accept_visitor(*this);
    }

    void visit(ExprCos& expr)
    {
        expr.expr->accept_visitor(*this);
    }

    void visit(ExprTan& expr)
    {
        expr.expr->accept_visitor(*this);
    }

    void visit(ExprCot& expr)
    {
        expr.expr->accept_visitor(*this);
    }

    void visit(ExprSinh& expr)
    {
        expr.expr->accept_visitor(*this);
    }

    void visit(ExprCosh& expr)
    {
        expr.expr->accept_visitor(*this);
    }

    void visit(ExprTanh& expr)
    {
        expr.expr->accept_visitor(*this);
    }

    void visit(ExprAsin& expr)
    {
        expr.expr->accept_visitor(*this);
    }

    void visit(ExprAcos& expr)
    {
        expr.expr->accept_visitor(*this);
    }

    void visit(ExprAtan& expr)
    {
        expr.expr->accept_visitor(*this);
    }

    void visit(ExprDegrees& expr)
    {
        expr.expr->accept_visitor(*this);
    }

    void visit(ExprRadians& expr)
    {
        expr.expr->accept_visitor(*this);
    }

    void visit(ExprLog& expr)
    {
        expr.lhs->accept_visitor(*this);
        expr.rhs->accept_visitor(*this);
    }

    void visit(ExprLog10& expr)
    {
        expr.expr->accept_visitor(*this);
    }

    void visit(ExprLn& expr)
    {
        expr.expr->accept_visitor(*this);
    }

    void visit(ExprExp& expr)
    {
        expr.expr->accept_visitor(*this);
    }

    void visit(ExprPower& expr)
    {
        expr.lhs->accept_visitor(*this);
        expr.rhs->accept_visitor(*this);
    }

    void visit(ExprSqrt& expr)
    {
        expr.expr->accept_visitor(*this);
    }

    void visit(ExprFloor& expr)
    {
        expr.expr->accept_visitor(*this);
    }

    void visit(ExprCeil& expr)
    {
        expr.expr->accept_visitor(*this);
    }

    void visit(ExprLength& expr)
    {
        expr.expr->accept_visitor(*this);
    }

    void visit(ExprSubStr& expr)
    {
        expr.expr->accept_visitor(*this);
        expr.str_len->accept_visitor(*this);
    }

    void visit(ExprFold& expr)
    {
        expr.expr->accept_visitor(*this);
    }

    void visit(ExprSingleTrim& expr)
    {
        expr.str->accept_visitor(*this);
        if (expr.single_char != nullptr) {
            expr.single_char->accept_visitor(*this);
        }
    }

    void visit(ExprMultiTrim& expr)
    {
        expr.trim_src->accept_visitor(*this);
        if (expr.delim_str != nullptr) {
            expr.delim_str->accept_visitor(*this);
        }
    }

    void visit(ExprNormalize& expr)
    {
        expr.expr->accept_visitor(*this);
    }

    void visit(ExprNullIf& expr)
    {
        expr.lhs->accept_visitor(*this);
        expr.rhs->accept_visitor(*this);
    }

    void visit(ExprCoalesce& expr)
    {
        for (const auto& expression : expr.expressions) {
            if (expression != nullptr) {
                expression->accept_visitor(*this);
            }
        }
    }

    void visit(ExprSimpleCase& expr)
    {
        expr.case_operand->accept_visitor(*this);
        for (const auto& clauses : expr.when_clauses) {
            for (const auto& clause : clauses.first.second) {
                clause->accept_visitor(*this);
            }
            clauses.second->accept_visitor(*this);
        }
        if (expr.else_expr != nullptr) {
            expr.else_expr->accept_visitor(*this);
        }
    }

    void visit(ExprSearchedCase& expr)
    {
        for (const auto& clause : expr.when_clauses) {
            clause.first->accept_visitor(*this);
            clause.second->accept_visitor(*this);
        }
        if (expr.else_clause != nullptr) {
            expr.else_clause->accept_visitor(*this);
        }
    }

    void visit(ExprCast& expr)
    {
        expr.operand->accept_visitor(*this);
    }

    void visit(ExprAggCount& expr)
    {
        expr.expr->accept_visitor(*this);
    }

    void visit(ExprAggAvg& expr)
    {
        expr.expr->accept_visitor(*this);
    }

    void visit(ExprAggMax& expr)
    {
        expr.expr->accept_visitor(*this);
    }

    void visit(ExprAggMin& expr)
    {
        expr.expr->accept_visitor(*this);
    }

    void visit(ExprAggSum& expr)
    {
        expr.expr->accept_visitor(*this);
    }

    void visit(ExprAggStddevPop& expr)
    {
        expr.expr->accept_visitor(*this);
    }

    void visit(ExprAggStddevSamp& expr)
    {
        expr.expr->accept_visitor(*this);
    }

    void visit(ExprAggCollect& expr)
    {
        expr.expr->accept_visitor(*this);
    }

    void visit(ExprAggPercentileCont& expr)
    {
        expr.expr->accept_visitor(*this);
    }

    void visit(ExprAggPercentileDisc& expr)
    {
        expr.expr->accept_visitor(*this);
    }

    void visit(ExprAggCountAll&) { }

    void visit(ExprTerm&) { }
    void visit(ExprHasNodeLabel&) { }
    void visit(ExprHasEdgeLabel&) { }
    void visit(ExprWildcardLabel&) { }
    void visit(ExprVar&) { }
    void visit(ExprIs&) { }
};
