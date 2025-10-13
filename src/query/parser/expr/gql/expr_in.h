#pragma once

#include "graph_models/object_id.h"
#include "query/parser/expr/gql/expr.h"

namespace GQL {
class ExprIn : public Expr {
public:
    std::unique_ptr<Expr> lhs;
    std::unique_ptr<Expr> rhs;

    ExprIn(std::unique_ptr<Expr> lhs, std::unique_ptr<Expr> rhs) :
        lhs(std::move(lhs)),
        rhs(std::move(rhs))
    { }

    void accept_visitor(ExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    bool has_aggregation() const override
    {
        return lhs->has_aggregation() || rhs->has_aggregation();
    }

    virtual std::unique_ptr<Expr> clone() const override
    {
        return std::make_unique<ExprIn>(lhs->clone(), rhs->clone());
    }

    std::set<VarId> get_all_vars() const override
    {
        std::set<VarId> vars = lhs->get_all_vars();
        vars.merge(rhs->get_all_vars());
        return vars;
    }
};
} // namespace GQL
