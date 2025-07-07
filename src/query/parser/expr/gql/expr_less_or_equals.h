#pragma once

#include "query/parser/expr/gql/expr.h"

namespace GQL {
class ExprLessOrEquals : public Expr {
public:
    std::unique_ptr<Expr> lhs;
    std::unique_ptr<Expr> rhs;

    ExprLessOrEquals(std::unique_ptr<Expr> lhs, std::unique_ptr<Expr> rhs) :
        lhs(std::move(lhs)),
        rhs(std::move(rhs))
    { }

    virtual std::unique_ptr<Expr> clone() const override
    {
        return std::make_unique<ExprLessOrEquals>(lhs->clone(), rhs->clone());
    }

    void accept_visitor(ExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    bool has_aggregation() const override
    {
        return lhs->has_aggregation() || rhs->has_aggregation();
    }

    std::set<VarId> get_all_vars() const override
    {
        std::set<VarId> res = lhs->get_all_vars();
        for (auto& var : rhs->get_all_vars()) {
            res.insert(var);
        }
        return res;
    }
};
} // namespace GQL
