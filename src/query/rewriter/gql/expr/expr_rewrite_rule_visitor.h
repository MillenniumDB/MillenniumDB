#pragma once

#include "query/parser/expr/expr_visitor.h"
#include "query/parser/expr/gql_exprs.h"
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
        for (auto& rule : rules) {
            for (auto& child : expr.exprs) {
                if (rule->is_possible_to_regroup(child)) {
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

    void visit(GQL::ExprOr& expr) override
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
            if (child != nullptr) {
                child->accept_visitor(*this);
            }
        }
    }

    void visit(GQL::ExprNot& expr) override
    {
        for (auto& rule : rules) {
            if (rule->is_possible_to_regroup(expr.expr)) {
                expr.expr = rule->regroup(std::move(expr.expr));
                has_rewritten = true;
            }
        }
        if (expr.expr != nullptr)
            expr.expr->accept_visitor(*this);
    }

    void visit(GQL::ExprEquals& expr) override
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

        if (expr.lhs != nullptr) {
            expr.lhs->accept_visitor(*this);
        }
        if (expr.rhs != nullptr) {
            expr.rhs->accept_visitor(*this);
        }
    }

    void visit(GQL::ExprXor& expr) override
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

        if (expr.lhs != nullptr) {
            expr.lhs->accept_visitor(*this);
        }
        if (expr.rhs != nullptr) {
            expr.rhs->accept_visitor(*this);
        }
    }

    void visit(GQL::ExprConcat& expr) override
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

        if (expr.lhs != nullptr) {
            expr.lhs->accept_visitor(*this);
        }
        if (expr.rhs != nullptr) {
            expr.rhs->accept_visitor(*this);
        }
    }

    void visit(GQL::ExprGreaterOrEquals& expr) override
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

        if (expr.lhs != nullptr) {
            expr.lhs->accept_visitor(*this);
        }
        if (expr.rhs != nullptr) {
            expr.rhs->accept_visitor(*this);
        }
    }

    void visit(GQL::ExprGreater& expr) override
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

        if (expr.lhs != nullptr) {
            expr.lhs->accept_visitor(*this);
        }
        if (expr.rhs != nullptr) {
            expr.rhs->accept_visitor(*this);
        }
    }

    void visit(GQL::ExprLessOrEquals& expr) override
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

        if (expr.lhs != nullptr) {
            expr.lhs->accept_visitor(*this);
        }
        if (expr.rhs != nullptr) {
            expr.rhs->accept_visitor(*this);
        }
    }

    void visit(GQL::ExprLess& expr) override
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

        if (expr.lhs != nullptr) {
            expr.lhs->accept_visitor(*this);
        }
        if (expr.rhs != nullptr) {
            expr.rhs->accept_visitor(*this);
        }
    }
    void visit(GQL::ExprNotEquals& expr) override
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

        if (expr.lhs != nullptr) {
            expr.lhs->accept_visitor(*this);
        }
        if (expr.rhs != nullptr) {
            expr.rhs->accept_visitor(*this);
        }
    }

    void visit(GQL::ExprIs& expr) override
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

        if (expr.lhs != nullptr) {
            expr.lhs->accept_visitor(*this);
        }
        if (expr.rhs != nullptr) {
            expr.rhs->accept_visitor(*this);
        }
    }

    void visit(GQL::ExprTerm&) override { }
    void visit(GQL::ExprHasNodeLabel&) override { }
    void visit(GQL::ExprHasEdgeLabel&) override { }
    void visit(GQL::ExprVar&) override { }
    void visit(GQL::ExprProperty&) override { }
};

} // namespace GQL
