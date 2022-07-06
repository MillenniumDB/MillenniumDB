#pragma once

#include <memory>

#include "parser/query/expr/expr.h"

class ExprUnaryMinus : public Expr {
public:
    std::unique_ptr<Expr> expr;

    ExprUnaryMinus(std::unique_ptr<Expr> expr) :
        expr (std::move(expr)) { }

    void accept_visitor(ExprVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<Var> get_vars() const override {
        return expr->get_vars();
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        return os << std::string(' ', indent) << "-(" <<*expr << ')' ;
    }
};
