#pragma once

#include <memory>

#include "query/parser/expr/expr.h"

namespace GQL {
class ExprCoalesce : public Expr {
public:
    std::vector<std::unique_ptr<Expr>> expressions;

    ExprCoalesce(std::vector<std::unique_ptr<Expr>> expressions) :
        expressions(std::move(expressions))
    { }

    virtual std::unique_ptr<Expr> clone() const override
    {
        std::vector<std::unique_ptr<Expr>> cloned_expressions;

        for (const auto& expr : expressions) {
            cloned_expressions.push_back(expr ? expr->clone() : nullptr);
        }

        return std::make_unique<ExprCoalesce>(std::move(cloned_expressions));
    }

    void accept_visitor(ExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    bool has_aggregation() const override
    {
        bool agg = false;
        for (const auto& expr : expressions) {
            if (expr->has_aggregation()) {
                agg = true;
                break;
            }
        }
        return agg;
    }

    std::set<VarId> get_all_vars() const override
    {
        std::set<VarId> res;
        for (const auto& expr : expressions) {
            if (expr != nullptr) {
                for (auto& var : expr->get_all_vars()) {
                    res.insert(var);
                }
            }
        }
        return res;
    }
};
} // namespace GQL
