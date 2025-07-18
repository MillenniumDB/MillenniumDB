#pragma once

#include "query/parser/expr/gql/expr_visitor.h"
#include "query/parser/expr/gql/exprs.h"
#include "query/rewriter/gql/expr/rewrite_rules/expr_rewrite_rule.h"

namespace GQL {

class ExprRewriteRuleVisitor : public ExprVisitor {
private:
    std::vector<std::unique_ptr<ExprRewriteRule>> rules;
    bool has_rewritten = false;

public:
    template<typename Rule>
    void add_rule()
    {
        rules.push_back(std::make_unique<Rule>());
    }

    void start_visit(std::unique_ptr<Expr>& expr)
    {
        auto temp_expr_container = std::make_unique<ExprNot>(std::move(expr));
        temp_expr_container->accept_visitor(*this);
        expr = std::move(temp_expr_container->expr);
    }

    bool reset_and_check_if_has_rewritten_a_rule()
    {
        auto out = has_rewritten;
        has_rewritten = false;
        return out;
    }

    void visit(GQL::ExprAnd& expr) override
    {
        visit_expr_with_exprs<ExprAnd>(expr);
    }

    void visit(GQL::ExprOr& expr) override
    {
        visit_expr_with_exprs<ExprOr>(expr);
    }

    void visit(GQL::ExprNot& expr) override
    {
        visit_expr_with_expr<ExprNot>(expr);
    }

    void visit(GQL::ExprEquals& expr) override
    {
        visit_expr_with_lhs_and_rhs<ExprEquals>(expr);
    }

    void visit(GQL::ExprXor& expr) override
    {
        visit_expr_with_lhs_and_rhs<ExprXor>(expr);
    }

    void visit(GQL::ExprConcat& expr) override
    {
        visit_expr_with_lhs_and_rhs<ExprConcat>(expr);
    }

    void visit(GQL::ExprGreaterOrEquals& expr) override
    {
        visit_expr_with_lhs_and_rhs<ExprGreaterOrEquals>(expr);
    }

    void visit(GQL::ExprGreater& expr) override
    {
        visit_expr_with_lhs_and_rhs<ExprGreater>(expr);
    }

    void visit(GQL::ExprLessOrEquals& expr) override
    {
        visit_expr_with_lhs_and_rhs<ExprLessOrEquals>(expr);
    }

    void visit(GQL::ExprLess& expr) override
    {
        visit_expr_with_lhs_and_rhs<ExprLess>(expr);
    }

    void visit(GQL::ExprNotEquals& expr) override
    {
        visit_expr_with_lhs_and_rhs<ExprNotEquals>(expr);
    }

    void visit(GQL::ExprIs& expr) override
    {
        visit_expr_with_lhs_and_rhs<ExprIs>(expr);
    }

    void visit(GQL::ExprAbs& expr) override
    {
        visit_expr_with_expr<ExprAbs>(expr);
    }

    void visit(GQL::ExprAcos& expr) override
    {
        visit_expr_with_expr<ExprAcos>(expr);
    }

    void visit(GQL::ExprAddition& expr) override
    {
        visit_expr_with_lhs_and_rhs<ExprAddition>(expr);
    }

    void visit(GQL::ExprAsin& expr) override
    {
        visit_expr_with_expr<ExprAsin>(expr);
    }

    void visit(GQL::ExprAtan& expr) override
    {
        visit_expr_with_expr<ExprAtan>(expr);
    }

    void visit(GQL::ExprCast& expr) override
    {
        visit_expr_with_expr<ExprCast>(expr);
    }

    void visit(GQL::ExprCeil& expr) override
    {
        visit_expr_with_expr<ExprCeil>(expr);
    }

    void visit(GQL::ExprCoalesce& expr) override
    {
        for (auto& rule : rules) {
            for (auto& child : expr.exprs) {
                if (child != nullptr && rule->is_possible_to_regroup(child)) {
                    child = rule->regroup(std::move(child));
                    has_rewritten = true;
                }
            }
        }
        for (auto& child : expr.exprs) {
            if (child != nullptr) {
                child->accept_visitor(*this);
            }
        }
    }

    void visit(GQL::ExprCos& expr) override
    {
        visit_expr_with_expr<ExprCos>(expr);
    }

    void visit(GQL::ExprCosh& expr) override
    {
        visit_expr_with_expr<ExprCosh>(expr);
    }

    void visit(GQL::ExprCot& expr) override
    {
        visit_expr_with_expr<ExprCot>(expr);
    }

    void visit(GQL::ExprDegrees& expr) override
    {
        visit_expr_with_expr<ExprDegrees>(expr);
    }

    void visit(GQL::ExprDivision& expr) override
    {
        visit_expr_with_lhs_and_rhs<ExprDivision>(expr);
    }

    void visit(GQL::ExprExp& expr) override
    {
        visit_expr_with_expr<ExprExp>(expr);
    }

    void visit(GQL::ExprFloor& expr) override
    {
        visit_expr_with_expr<ExprFloor>(expr);
    }

    void visit(GQL::ExprFold& expr) override
    {
        visit_expr_with_expr<ExprFold>(expr);
    }

    void visit(GQL::ExprLength& expr) override
    {
        visit_expr_with_expr<ExprLength>(expr);
    }

    void visit(GQL::ExprLn& expr) override
    {
        visit_expr_with_expr<ExprLn>(expr);
    }

    void visit(GQL::ExprLog& expr) override
    {
        visit_expr_with_lhs_and_rhs<ExprLog>(expr);
    }

    void visit(GQL::ExprLog10& expr) override
    {
        visit_expr_with_expr<ExprLog10>(expr);
    }

    void visit(GQL::ExprModulo& expr) override
    {
        visit_expr_with_lhs_and_rhs<ExprModulo>(expr);
    }

    void visit(GQL::ExprMultiplication& expr) override
    {
        visit_expr_with_lhs_and_rhs<ExprMultiplication>(expr);
    }

    void visit(GQL::ExprMultiTrim& expr) override
    {
        for (auto& rule : rules) {
            if (rule->is_possible_to_regroup(expr.trim_src)) {
                expr.trim_src = rule->regroup(std::move(expr.trim_src));
                has_rewritten = true;
            }
            if (expr.delim_str != nullptr && rule->is_possible_to_regroup(expr.delim_str)) {
                expr.delim_str = rule->regroup(std::move(expr.delim_str));
                has_rewritten = true;
            }
        }

        expr.trim_src->accept_visitor(*this);
        if (expr.delim_str != nullptr) {
            expr.delim_str->accept_visitor(*this);
        }
    }

    void visit(GQL::ExprNormalize& expr) override
    {
        visit_expr_with_expr<ExprNormalize>(expr);
    }

    void visit(GQL::ExprNullIf& expr) override
    {
        visit_expr_with_lhs_and_rhs<ExprNullIf>(expr);
    }

    void visit(GQL::ExprPower& expr) override
    {
        visit_expr_with_lhs_and_rhs<ExprPower>(expr);
    }

    void visit(GQL::ExprRadians& expr) override
    {
        visit_expr_with_expr<ExprRadians>(expr);
    }

    void visit(GQL::ExprSearchedCase& expr) override
    {
        for (auto& rule : rules) {
            for (auto& [k, v] : expr.when_clauses) {
                if (rule->is_possible_to_regroup(k)) {
                    k = rule->regroup(std::move(k));
                    has_rewritten = true;
                }
                if (rule->is_possible_to_regroup(v)) {
                    v = rule->regroup(std::move(v));
                    has_rewritten = true;
                }
            }
            if (expr.else_clause != nullptr && rule->is_possible_to_regroup(expr.else_clause)) {
                expr.else_clause = rule->regroup(std::move(expr.else_clause));
                has_rewritten = true;
            }
        }
        for (auto& [k, v] : expr.when_clauses) {
            k->accept_visitor(*this);
            v->accept_visitor(*this);
        }
        if (expr.else_clause != nullptr) {
            expr.else_clause->accept_visitor(*this);
        }
    }

    void visit(GQL::ExprSimpleCase& expr) override
    {
        for (auto& rule : rules) {
            if (rule->is_possible_to_regroup(expr.case_operand)) {
                expr.case_operand = rule->regroup(std::move(expr.case_operand));
                has_rewritten = true;
            }
            for (auto& child : expr.when_clauses) {
                for (auto& sub_expr : child.first.second) {
                    if (rule->is_possible_to_regroup(sub_expr)) {
                        sub_expr = rule->regroup(std::move(sub_expr));
                        has_rewritten = true;
                    }
                }
                if (rule->is_possible_to_regroup(child.second)) {
                    child.second = rule->regroup(std::move(child.second));
                    has_rewritten = true;
                }
            }
            if (expr.else_expr != nullptr && rule->is_possible_to_regroup(expr.else_expr)) {
                expr.else_expr = rule->regroup(std::move(expr.else_expr));
                has_rewritten = true;
            }
        }

        expr.case_operand->accept_visitor(*this);

        for (auto& child : expr.when_clauses) {
            for (auto& sub_expr : child.first.second) {
                sub_expr->accept_visitor(*this);
            }
            child.second->accept_visitor(*this);
        }
        if (expr.else_expr != nullptr) {
            expr.else_expr->accept_visitor(*this);
        }
    }

    void visit(GQL::ExprSin& expr) override
    {
        visit_expr_with_expr<ExprSin>(expr);
    }

    void visit(GQL::ExprSingleTrim& expr) override
    {
        for (auto& rule : rules) {
            if (rule->is_possible_to_regroup(expr.str)) {
                expr.str = rule->regroup(std::move(expr.str));
                has_rewritten = true;
            }
            if (expr.single_char != nullptr && rule->is_possible_to_regroup(expr.single_char)) {
                expr.single_char = rule->regroup(std::move(expr.single_char));
                has_rewritten = true;
            }
        }

        expr.str->accept_visitor(*this);
        if (expr.single_char != nullptr) {
            expr.single_char->accept_visitor(*this);
        }
    }

    void visit(GQL::ExprSinh& expr) override
    {
        visit_expr_with_expr<ExprSinh>(expr);
    }

    void visit(GQL::ExprSqrt& expr) override
    {
        visit_expr_with_expr<ExprSqrt>(expr);
    }

    void visit(GQL::ExprSubStr& expr) override
    {
        visit_expr_with_expr<ExprSubStr>(expr);
    }

    void visit(GQL::ExprSubtraction& expr) override
    {
        visit_expr_with_lhs_and_rhs<ExprSubtraction>(expr);
    }

    void visit(GQL::ExprTan& expr) override
    {
        visit_expr_with_expr<ExprTan>(expr);
    }

    void visit(GQL::ExprTanh& expr) override
    {
        visit_expr_with_expr<ExprTanh>(expr);
    }

    virtual void visit(GQL::ExprAggCount& expr) override
    {
        visit_expr_with_expr<ExprAggCount>(expr);
    }

    virtual void visit(GQL::ExprAggAvg& expr) override
    {
        visit_expr_with_expr<ExprAggAvg>(expr);
    }

    virtual void visit(GQL::ExprAggMax& expr) override
    {
        visit_expr_with_expr<ExprAggMax>(expr);
    }

    virtual void visit(GQL::ExprAggMin& expr) override
    {
        visit_expr_with_expr<ExprAggMin>(expr);
    }

    virtual void visit(GQL::ExprAggSum& expr) override
    {
        visit_expr_with_expr<ExprAggSum>(expr);
    }

    virtual void visit(GQL::ExprAggStddevPop& expr) override
    {
        visit_expr_with_expr<ExprAggStddevPop>(expr);
    }

    virtual void visit(GQL::ExprAggStddevSamp& expr) override
    {
        visit_expr_with_expr<ExprAggStddevSamp>(expr);
    }

    virtual void visit(GQL::ExprAggCollect& expr) override
    {
        visit_expr_with_expr<ExprAggCollect>(expr);
    }

    virtual void visit(GQL::ExprAggPercentileCont& expr) override
    {
        visit_expr_with_expr<ExprAggPercentileCont>(expr);
    }

    virtual void visit(GQL::ExprAggPercentileDisc& expr) override
    {
        visit_expr_with_expr<ExprAggPercentileDisc>(expr);
    }

    virtual void visit(GQL::ExprAggCountAll&) override { }
    void visit(GQL::ExprTerm&) override { }
    void visit(GQL::ExprHasNodeLabel&) override { }
    void visit(GQL::ExprHasEdgeLabel&) override { }
    void visit(GQL::ExprWildcardLabel&) override { }
    void visit(GQL::ExprVar&) override { }
    void visit(GQL::ExprProperty&) override { }

private:
    template<typename T>
    void visit_expr_with_expr(T& expr)
    {
        for (auto& rule : rules) {
            if (rule->is_possible_to_regroup(expr.expr)) {
                expr.expr = rule->regroup(std::move(expr.expr));
                has_rewritten = true;
            }
        }
        expr.expr->accept_visitor(*this);
    }

    template<typename T>
    void visit_expr_with_exprs(T& expr)
    {
        for (auto& rule : rules) {
            for (auto& child : expr.exprs) {
                if (rule->is_possible_to_regroup(child)) {
                    child = rule->regroup(std::move(child));
                    has_rewritten = true;
                }
            }
        }
        for (auto& child : expr.exprs) {
            child->accept_visitor(*this);
        }
    }

    template<typename T>
    void visit_expr_with_lhs_and_rhs(T& expr)
    {
        for (auto& rule : rules) {
            if (rule->is_possible_to_regroup(expr.lhs)) {
                expr.lhs = rule->regroup(std::move(expr.lhs));
                has_rewritten = true;
            }
            if (rule->is_possible_to_regroup(expr.rhs)) {
                expr.rhs = rule->regroup(std::move(expr.rhs));
                has_rewritten = true;
            }
        }

        expr.lhs->accept_visitor(*this);
        expr.rhs->accept_visitor(*this);
    }
};

} // namespace GQL
