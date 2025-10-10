#pragma once

#include "graph_models/object_id.h"
#include "query/parser/expr/gql/expr.h"

namespace GQL {
class ExprIn : public Expr {
public:
    std::unique_ptr<Expr> expr;
    ObjectId list_id;

    ExprIn(std::unique_ptr<Expr> expr, ObjectId list_id) :
        expr(std::move(expr)),
        list_id(list_id)
    { }

    void accept_visitor(ExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    bool has_aggregation() const override
    {
        return expr->has_aggregation();
    }

    virtual std::unique_ptr<Expr> clone() const override
    {
        return std::make_unique<ExprIn>(expr->clone(), list_id);
    }

    std::set<VarId> get_all_vars() const override
    {
        return expr->get_all_vars();
    }
};
} // namespace GQL
