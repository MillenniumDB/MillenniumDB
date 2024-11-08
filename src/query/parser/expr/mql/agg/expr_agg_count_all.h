#pragma once

#include "query/parser/expr/expr.h"

namespace MQL {
class ExprAggCountAll : public Expr {
public:
    bool distinct;

    ExprAggCountAll(bool distinct) :
        distinct (distinct) { }

    virtual std::unique_ptr<Expr> clone() const override {
        return std::make_unique<ExprAggCountAll>(distinct);
    }

    void accept_visitor(ExprVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override {
        return {};
    }

    bool has_aggregation() const override {
        return true;
    }
};
} // namespace MQL
