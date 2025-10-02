#pragma once

#include "query/parser/expr/mql/expr.h"

namespace MQL {

class ExprProperties : public Expr {
public:
    VarId var;

    ExprProperties(VarId var) :
        var(var)
    { }

    virtual std::unique_ptr<Expr> clone() const override
    {
        return std::make_unique<ExprProperties>(var);
    }

    void accept_visitor(ExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override
    {
        return { var };
    }

    std::set<VarId> get_input_vars() const override
    {
        return { var };
    }

    bool has_aggregation() const override
    {
        return false;
    }
};
} // namespace MQL
