#pragma once

#include <vector>

#include "query/parser/expr/gql/expr.h"

namespace GQL {
class ExprSearchedCase : public Expr {
public:
    std::vector<std::pair<std::unique_ptr<Expr>, std::unique_ptr<Expr>>> when_clauses;
    std::unique_ptr<Expr> else_clause;

    ExprSearchedCase(
        std::vector<std::pair<std::unique_ptr<Expr>, std::unique_ptr<Expr>>> when_clauses,
        std::unique_ptr<Expr> else_clause
    ) :
        when_clauses(std::move(when_clauses)),
        else_clause(std::move(else_clause))
    { }

    virtual std::unique_ptr<Expr> clone() const override
    {
        std::vector<std::pair<std::unique_ptr<Expr>, std::unique_ptr<Expr>>> cloned_when_clauses;

        for (const auto& clause : when_clauses) {
            auto cloned_condition = clause.first->clone();
            auto cloned_result = clause.second->clone();

            cloned_when_clauses.emplace_back(
                std::make_pair(std::move(cloned_condition), std::move(cloned_result))
            );
        }

        return std::make_unique<ExprSearchedCase>(std::move(cloned_when_clauses), else_clause->clone());
    }

    void accept_visitor(ExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    bool has_aggregation() const override
    {
        for (const auto& clause : when_clauses) {
            if (clause.first->has_aggregation() || clause.second->has_aggregation()) {
                return true;
            }
        }

        if (else_clause && else_clause->has_aggregation()) {
            return true;
        }

        return false;
    }

    std::set<VarId> get_all_vars() const override
    {
        std::set<VarId> res;

        for (const auto& clause : when_clauses) {
            for (auto& var : clause.first->get_all_vars()) {
                res.insert(var);
            }
            for (auto& var : clause.second->get_all_vars()) {
                res.insert(var);
            }
        }

        if (else_clause) {
            for (auto& var : else_clause->get_all_vars()) {
                res.insert(var);
            }
        }

        return res;
    }
};
} // namespace GQL
