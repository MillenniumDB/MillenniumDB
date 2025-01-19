#pragma once

#include <memory>
#include <vector>

#include "query/parser/expr/expr.h"

namespace GQL {
class ExprAnd : public Expr {
public:
    std::vector<std::unique_ptr<Expr>> exprs;

    ExprAnd(std::vector<std::unique_ptr<Expr>>&& and_list) :
        exprs(std::move(and_list))
    { }

    virtual std::unique_ptr<Expr> clone() const override
    {
        std::vector<std::unique_ptr<Expr>> and_list_clone;
        and_list_clone.reserve(exprs.size());
        for (auto& child_expr : exprs) {
            and_list_clone.push_back(child_expr->clone());
        }
        return std::make_unique<ExprAnd>(std::move(and_list_clone));
    }

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
