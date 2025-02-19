#pragma once

#include <memory>

#include "query/parser/expr/expr.h"

namespace GQL {
class ExprAggCountAll : public Expr {
public:
    ExprAggCountAll() { }

    virtual std::unique_ptr<Expr> clone() const override {
        return std::make_unique<ExprAggCountAll>();
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
} // namespace GQL
