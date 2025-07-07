#pragma once

#include <memory>

#include "query/parser/expr/sparql/expr.h"
#include "query/parser/op/sparql/op.h"

namespace SPARQL {
class ExprNotExists : public Expr {
public:
    std::unique_ptr<Op> op;

    ExprNotExists(std::unique_ptr<Op> op) :
        op (std::move(op)) { }

    virtual std::unique_ptr<Expr> clone() const override {
        return std::make_unique<ExprNotExists>(op->clone());
    }

    void accept_visitor(ExprVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override {
        return op->get_all_vars();
    }

    bool has_aggregation() const override {
        return false;
    }
};
} // namespace SPARQL
