#pragma once

#include "query/parser/expr/mql/expr.h"

namespace MQL {
class ExprModulo : public Expr {
public:
    std::unique_ptr<Expr> lhs;
    std::unique_ptr<Expr> rhs;

    ExprModulo(std::unique_ptr<Expr> lhs, std::unique_ptr<Expr> rhs) :
        lhs(std::move(lhs)),
        rhs(std::move(rhs))
    { }

    virtual std::unique_ptr<Expr> clone() const override
    {
        return std::make_unique<ExprModulo>(lhs->clone(), rhs->clone());
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
        auto rhs_vars = rhs->get_all_vars();
        res.insert(rhs_vars.begin(), rhs_vars.end());
        return res;
    }
};
} // namespace MQL
