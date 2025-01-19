#pragma once

#include <memory>
#include <vector>

#include "query/parser/expr/expr.h"

namespace GQL {
class ExprOr : public Expr {
public:
    std::vector<std::unique_ptr<Expr>> exprs;

    ExprOr(std::vector<std::unique_ptr<Expr>>&& or_list) :
        exprs(std::move(or_list))
    { }

    void accept_visitor(ExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    bool has_aggregation() const override
    {
        for (auto& expr : exprs) {
            if (expr->has_aggregation())
                return true;
        }
        return false;
    }

    virtual std::unique_ptr<Expr> clone() const override
    {
        std::vector<std::unique_ptr<Expr>> or_list_clone;
        or_list_clone.reserve(exprs.size());
        for (auto& child_expr : exprs) {
            or_list_clone.push_back(child_expr->clone());
        }
        return std::make_unique<ExprOr>(std::move(or_list_clone));
    }

    std::set<VarId> get_all_vars() const override
    {
        std::set<VarId> res;
        for (auto& expr : exprs) {
            for (auto& var : expr->get_all_vars()) {
                res.insert(var);
            }
        }
        return res;
    }
};
} // namespace GQL
