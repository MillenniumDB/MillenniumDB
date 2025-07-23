#pragma once

#include "query/parser/expr/gql/exprs.h"
#include "query/query_context.h"

namespace GQL {

class CheckGroupVars : public ExprVisitor {
private:
    std::set<VarId>& group_vars;

public:
    CheckGroupVars(std::set<VarId>& group_vars) :
        group_vars(group_vars)
    { }

    void visit(ExprAggCountAll&) override { }
    void visit(ExprAggCount&) override { }
    void visit(ExprAggAvg&) override { }
    void visit(ExprAggMax&) override { }
    void visit(ExprAggMin&) override { }
    void visit(ExprAggSum&) override { }
    void visit(ExprAggStddevPop&) override { }
    void visit(ExprAggStddevSamp&) override { }
    void visit(ExprAggCollect&) override { }
    void visit(ExprAggPercentileCont&) override { }
    void visit(ExprAggPercentileDisc&) override { }
    void visit(ExprTerm&) override { }

    void visit(ExprVar& expr) override
    {
        check_group_var(expr.id);
    }

    void visit(ExprProperty& expr) override
    {
        check_group_var(expr.value);
    }

    void visit(ExprHasNodeLabel& expr) override
    {
        check_group_var(expr.node_id);
    }

    void visit(ExprHasEdgeLabel& expr) override
    {
        check_group_var(expr.edge_id);
    }

    void visit(ExprWildcardLabel& expr) override
    {
        check_group_var(expr.var_id);
    }

    void check_group_var(VarId var_id)
    {
        if (!group_vars.count(var_id)) {
            throw QuerySemanticException(
                "The variable \"" + get_query_ctx().get_var_name(var_id)
                + "\" is not a group variable and it is not within an aggregate function."
            );
        }
    }

    void visit(ExprSubStr& expr) override
    {
        expr.expr->accept_visitor(*this);
        expr.str_len->accept_visitor(*this);
    }

    void visit(ExprSingleTrim& expr) override
    {
        expr.str->accept_visitor(*this);
        if (expr.single_char != nullptr) {
            expr.single_char->accept_visitor(*this);
        }
    }

    void visit(ExprMultiTrim& expr) override
    {
        expr.trim_src->accept_visitor(*this);
        if (expr.delim_str != nullptr) {
            expr.delim_str->accept_visitor(*this);
        }
    }

    void visit(ExprCoalesce& expr) override
    {
        for (auto& child : expr.exprs) {
            if (child != nullptr) {
                child->accept_visitor(*this);
            }
        }
    }

    void visit(ExprSimpleCase& expr) override
    {
        expr.case_operand->accept_visitor(*this);
        for (auto& clause : expr.when_clauses) {
            for (auto& child : clause.first.second) {
                child->accept_visitor(*this);
            }
            clause.second->accept_visitor(*this);
        }
        if (expr.else_expr != nullptr) {
            expr.else_expr->accept_visitor(*this);
        }
    }

    void visit(ExprSearchedCase& expr) override
    {
        for (auto& clause : expr.when_clauses) {
            clause.first->accept_visitor(*this);
            clause.second->accept_visitor(*this);
        }
        if (expr.else_clause != nullptr) {
            expr.else_clause->accept_visitor(*this);
        }
    }

    void visit(ExprAnd& expr) override
    {
        for (auto& child : expr.exprs) {
            child->accept_visitor(*this);
        }
    }

    void visit(ExprOr& expr) override
    {
        for (auto& child : expr.exprs) {
            child->accept_visitor(*this);
        }
    }

    void visit(ExprNot& expr) override
    {
        expr.expr->accept_visitor(*this);
    }

    void visit(ExprAddition& expr) override
    {
        expr.lhs->accept_visitor(*this);
        expr.rhs->accept_visitor(*this);
    }

    void visit(ExprGreater& expr) override
    {
        expr.lhs->accept_visitor(*this);
        expr.rhs->accept_visitor(*this);
    }
    void visit(ExprXor& expr) override
    {
        expr.lhs->accept_visitor(*this);
        expr.rhs->accept_visitor(*this);
    }

    void visit(ExprConcat& expr) override
    {
        expr.lhs->accept_visitor(*this);
        expr.rhs->accept_visitor(*this);
    }

    void visit(ExprGreaterOrEquals& expr) override
    {
        expr.lhs->accept_visitor(*this);
        expr.rhs->accept_visitor(*this);
    }

    void visit(ExprLessOrEquals& expr) override
    {
        expr.lhs->accept_visitor(*this);
        expr.rhs->accept_visitor(*this);
    }

    void visit(ExprLess& expr) override
    {
        expr.lhs->accept_visitor(*this);
        expr.rhs->accept_visitor(*this);
    }

    void visit(ExprSubtraction& expr) override
    {
        expr.lhs->accept_visitor(*this);
        expr.rhs->accept_visitor(*this);
    }

    void visit(ExprMultiplication& expr) override
    {
        expr.lhs->accept_visitor(*this);
        expr.rhs->accept_visitor(*this);
    }

    void visit(ExprDivision& expr) override
    {
        expr.lhs->accept_visitor(*this);
        expr.rhs->accept_visitor(*this);
    }

    void visit(ExprEquals& expr) override
    {
        expr.lhs->accept_visitor(*this);
        expr.rhs->accept_visitor(*this);
    }

    void visit(ExprNotEquals& expr) override
    {
        expr.lhs->accept_visitor(*this);
        expr.rhs->accept_visitor(*this);
    }

    void visit(ExprIs& expr) override
    {
        expr.lhs->accept_visitor(*this);
        expr.rhs->accept_visitor(*this);
    }

    void visit(ExprAbs& expr) override
    {
        expr.expr->accept_visitor(*this);
    }

    void visit(ExprModulo& expr) override
    {
        expr.lhs->accept_visitor(*this);
        expr.rhs->accept_visitor(*this);
    }

    void visit(ExprSin& expr) override
    {
        expr.expr->accept_visitor(*this);
    }

    void visit(ExprCos& expr) override
    {
        expr.expr->accept_visitor(*this);
    }

    void visit(ExprTan& expr) override
    {
        expr.expr->accept_visitor(*this);
    }

    void visit(ExprCot& expr) override
    {
        expr.expr->accept_visitor(*this);
    }

    void visit(ExprSinh& expr) override
    {
        expr.expr->accept_visitor(*this);
    }

    void visit(ExprCosh& expr) override
    {
        expr.expr->accept_visitor(*this);
    }

    void visit(ExprTanh& expr) override
    {
        expr.expr->accept_visitor(*this);
    }

    void visit(ExprAsin& expr) override
    {
        expr.expr->accept_visitor(*this);
    }

    void visit(ExprAcos& expr) override
    {
        expr.expr->accept_visitor(*this);
    }

    void visit(ExprAtan& expr) override
    {
        expr.expr->accept_visitor(*this);
    }

    void visit(ExprDegrees& expr) override
    {
        expr.expr->accept_visitor(*this);
    }

    void visit(ExprRadians& expr) override
    {
        expr.expr->accept_visitor(*this);
    }

    void visit(ExprLog& expr) override
    {
        expr.lhs->accept_visitor(*this);
        expr.rhs->accept_visitor(*this);
    }

    void visit(ExprLog10& expr) override
    {
        expr.expr->accept_visitor(*this);
    }

    void visit(ExprLn& expr) override
    {
        expr.expr->accept_visitor(*this);
    }

    void visit(ExprExp& expr) override
    {
        expr.expr->accept_visitor(*this);
    }

    void visit(ExprPower& expr) override
    {
        expr.lhs->accept_visitor(*this);
        expr.rhs->accept_visitor(*this);
    }

    void visit(ExprSqrt& expr) override
    {
        expr.expr->accept_visitor(*this);
    }

    void visit(ExprFloor& expr) override
    {
        expr.expr->accept_visitor(*this);
    }

    void visit(ExprCeil& expr) override
    {
        expr.expr->accept_visitor(*this);
    }

    void visit(ExprLength& expr) override
    {
        expr.expr->accept_visitor(*this);
    }

    void visit(ExprFold& expr) override
    {
        expr.expr->accept_visitor(*this);
    }

    void visit(ExprNormalize& expr) override
    {
        expr.expr->accept_visitor(*this);
    }

    void visit(ExprNullIf& expr) override
    {
        expr.lhs->accept_visitor(*this);
        expr.rhs->accept_visitor(*this);
    }

    void visit(ExprCast& expr) override
    {
        expr.expr->accept_visitor(*this);
    }
};

} // namespace GQL
