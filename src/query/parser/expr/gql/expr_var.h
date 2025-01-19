#pragma once

#include <memory>

#include "query/parser/expr/expr.h"

namespace GQL {
class ExprVar : public Expr {
public:
    VarId id;

    ExprVar(VarId id) :
        id(id)
    { }

    virtual std::unique_ptr<Expr> clone() const override
    {
        return std::make_unique<ExprVar>(id);
    }

    void accept_visitor(ExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    bool has_aggregation() const override
    {
        return false;
    }

    std::set<VarId> get_all_vars() const override
    {
        return { id };
    }
};
} // namespace GQL
