#pragma once

#include <memory>

#include "query/parser/expr/expr.h"

namespace SPARQL {
class ExprStrUUID : public Expr {
public:

    ExprStrUUID() = default;

    void accept_visitor(ExprVisitor& visitor) override {
        visitor.visit(*this);
    }

    virtual std::unique_ptr<Expr> clone() const override {
        return std::make_unique<ExprStrUUID>();
    }

    std::set<VarId> get_all_vars() const override {
        return {};
    }

    bool has_aggregation() const override {
        return false;
    }
};
} // namespace SPARQL
