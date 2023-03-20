#pragma once

#include "parser/query/expr/expr.h"

class ExprConstant : public Expr {
public:
    // e.g: true, false, "string", 123, 0.12
    std::string value; // TODO: use QueryElement?

    ExprConstant(const std::string& value) :
        value (value) { }

    ExprConstant(std::string&& value) :
        value (std::move(value)) { }

    void accept_visitor(ExprVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<Var> get_vars() const override {
        std::set<Var> res;
        return res;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        return os << std::string(' ', indent) << value;
    }
};
