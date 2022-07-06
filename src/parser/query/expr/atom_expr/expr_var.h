#pragma once

#include "parser/query/expr/expr.h"

class ExprVar : public Expr {
public:
    Var var;

    ExprVar(const std::string& var) :
        var (var) { }

    ExprVar(std::string&& var) :
        var (std::move(var)) { }

    void accept_visitor(ExprVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<Var> get_vars() const override {
        std::set<Var> res;
        res.insert(var);
        return res;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        return os << std::string(' ', indent) << var;
    }
};
