#pragma once

#include <memory>

#include "query/parser/expr/expr.h"

namespace GQL {
class ExprSimpleCase: public Expr {
public:
    std::unique_ptr<Expr> case_operand;
    std::vector<std::pair<std::pair<std::string, std::vector<std::unique_ptr<Expr>>>, std::unique_ptr<Expr>>> when_clauses;
    std::unique_ptr<Expr> else_expr;

    ExprSimpleCase(
        std::unique_ptr<Expr> case_operand,
        std::vector<std::pair<std::pair<std::string, std::vector<std::unique_ptr<Expr>>>, std::unique_ptr<Expr>>> when_clauses,
        std::unique_ptr<Expr> else_expr
    ) :
        case_operand(std::move(case_operand)),
        when_clauses(std::move(when_clauses)),
        else_expr(std::move(else_expr))
    { }

    virtual std::unique_ptr<Expr> clone() const override
    {
        std::vector<std::pair<std::pair<std::string, std::vector<std::unique_ptr<Expr>>>, std::unique_ptr<Expr>>> cloned_when_clauses;

        for (const auto& clause : when_clauses) {
            std::vector<std::unique_ptr<Expr>> cloned_vector;
            for (const auto& expr : clause.first.second) {
                cloned_vector.push_back(expr ? expr->clone() : nullptr);
            }

            std::unique_ptr<Expr> cloned_expr = clause.second ? clause.second->clone() : nullptr;

            cloned_when_clauses.emplace_back(std::make_pair(clause.first.first, std::move(cloned_vector)), std::move(cloned_expr));
        }

        return std::make_unique<ExprSimpleCase>(case_operand->clone(), std::move(cloned_when_clauses), else_expr->clone());
    }

    void accept_visitor(ExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    bool has_aggregation() const override
    {
        if (case_operand && case_operand->has_aggregation()) {
            return true;
        }

        for (const auto& clause : when_clauses) {
            for (const auto& expr : clause.first.second) {
                if (expr && expr->has_aggregation()) {
                    return true;
                }
            }
            if (clause.second && clause.second->has_aggregation()) {
                return true;
            }
        }

        if (else_expr && else_expr->has_aggregation()) {
            return true;
        }

        return false;
    }

    std::set<VarId> get_all_vars() const override
    {
        std::set<VarId> res;

        if (case_operand) {
            for (auto& var : case_operand->get_all_vars()) {
                res.insert(var);
            }
        }

        for (const auto& clause : when_clauses) {
            for (const auto& expr : clause.first.second) {
                if (expr) {
                    for (auto& var : expr->get_all_vars()) {
                        res.insert(var);
                    }
                }
            }
            if (clause.second) {
                for (auto& var : clause.second->get_all_vars()) {
                    res.insert(var);
                }
            }
        }

        if (else_expr) {
            for (auto& var : else_expr->get_all_vars()) {
                res.insert(var);
            }
        }

        return res;
    }
};
} // namespace GQL
