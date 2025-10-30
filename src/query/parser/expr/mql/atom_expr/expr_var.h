#pragma once

#include "query/parser/expr/mql/expr.h"

namespace MQL {
class ExprVar : public Expr {
public:
    VarId var;

    ExprVar(VarId var) :
        var(var)
    { }

    virtual std::unique_ptr<Expr> clone() const override
    {
        return std::make_unique<ExprVar>(var);
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
        return { var };
    }

    std::set<VarId> get_input_vars() const override
    {
        return { var };
    }

    std::optional<VarId> get_var() const override
    {
        return var;
    }

    void print(std::ostream& os) const override
    {
        os << var;
    }
};
} // namespace MQL
