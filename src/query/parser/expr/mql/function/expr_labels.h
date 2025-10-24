#pragma once

#include "query/parser/expr/mql/expr.h"

namespace MQL {

class ExprLabels : public Expr {
public:
    VarId var;

    ExprLabels(VarId node_id) :
        var(node_id)
    { }

    virtual std::unique_ptr<Expr> clone() const override
    {
        return std::make_unique<ExprLabels>(var);
    }

    void accept_visitor(ExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override
    {
        return {var};
    }

    bool has_aggregation() const override
    {
        return false;
    }
};
} // namespace MQL
