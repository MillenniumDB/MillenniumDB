#pragma once

#include "query/parser/expr/mql/expr.h"

namespace MQL {

class ExprType : public Expr {
public:
    VarId var;

    ExprType(VarId edge_id) :
        var(edge_id)
    { }

    virtual std::unique_ptr<Expr> clone() const override
    {
        return std::make_unique<ExprType>(var);
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

    void print(std::ostream& os) const override
    {
        os << "TYPE(" << var << ')';
    }
};
} // namespace MQL
