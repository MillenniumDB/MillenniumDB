#pragma once

#include "query/query_context.h"
#include "query/parser/expr/expr.h"

namespace MQL {
class ExprVar : public Expr {
public:
    VarId var;

    ExprVar(VarId var) :
        var (var) { }

    virtual std::unique_ptr<Expr> clone() const override {
        return std::make_unique<ExprVar>(var);
    }

    void accept_visitor(ExprVisitor& visitor) override {
        visitor.visit(*this);
    }

    bool has_aggregation() const override { return false; }

    std::set<VarId> get_all_vars() const override {
        return { var };
    }
};
} // namespace MQL
