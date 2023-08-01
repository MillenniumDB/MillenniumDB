#pragma once

#include "query/parser/expr/expr.h"

namespace MQL {
class ExprUnaryPlus : public Expr {
public:
    std::unique_ptr<Expr> expr;

    ExprUnaryPlus(std::unique_ptr<Expr> expr) :
        expr (std::move(expr)) { }

    virtual std::unique_ptr<Expr> clone() const override {
        return std::make_unique<ExprUnaryPlus>(expr->clone());
    }

    void accept_visitor(ExprVisitor& visitor) override {
        visitor.visit(*this);
    }

    bool has_aggregation() const override {
        return expr->has_aggregation();
    }

    std::set<VarId> get_all_vars() const override {
        return expr->get_all_vars();
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        return os << std::string(indent, ' ') << "+(" <<*expr << ')' ;
    }
};
} // namespace MQL
