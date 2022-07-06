#pragma once

#include <memory>

#include "parser/query/expr/expr.h"

class ExprGreaterOrEquals : public Expr {
public:
    std::unique_ptr<Expr> lhs;
    std::unique_ptr<Expr> rhs;

    ExprGreaterOrEquals(std::unique_ptr<Expr> lhs, std::unique_ptr<Expr> rhs) :
        lhs (std::move(lhs)),
        rhs (std::move(rhs)) { }

    void accept_visitor(ExprVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<Var> get_vars() const override {
        std::set<Var> res = lhs->get_vars();
        for (auto& var : rhs->get_vars()) {
            res.insert(var);
        }
        return res;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        return os << std::string(' ', indent) << '(' <<*lhs << " >= " << *rhs <<  ')' ;
    }
};
