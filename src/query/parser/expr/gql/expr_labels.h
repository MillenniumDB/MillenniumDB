#pragma once

#include "query/parser/expr/gql/expr.h"
#include "query/rewriter/gql/op/var_type.h"

namespace GQL {
class ExprLabels : public Expr {
public:
    VarId var;
    VarType::Type type;

    ExprLabels(VarId var, VarType::Type type) :
        var(var),
        type(type)
    { }

    virtual std::unique_ptr<Expr> clone() const override
    {
        return std::make_unique<ExprLabels>(var, type);
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
};
} // namespace GQL
