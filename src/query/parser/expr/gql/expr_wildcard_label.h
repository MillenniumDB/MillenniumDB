#pragma once

#include "query/parser/expr/gql/expr.h"
#include "query/rewriter/gql/op/var_type.h"

namespace GQL {
class ExprWildcardLabel : public Expr {
public:
    VarId var_id;
    VarType::Type type;

    ExprWildcardLabel(VarId var_id, VarType::Type type) :
        var_id(var_id),
        type(type)
    { }

    virtual std::unique_ptr<Expr> clone() const override
    {
        return std::make_unique<ExprWildcardLabel>(var_id, type);
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
        return { var_id };
    }
};
} // namespace GQL
