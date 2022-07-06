#pragma once

#include "parser/query/expr/expr.h"

class ExprVarProperty : public Expr {
public:
    Var object_var; // ?x
    Var property_var; // ?x.key
    std::string property_key; // key

    ExprVarProperty(const std::string& object_var, const std::string& property_var, const std::string& property_key) :
        object_var   (object_var),
        property_var (std::move(property_var)),
        property_key (std::move(property_key)) { }

    void accept_visitor(ExprVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<Var> get_vars() const override {
        std::set<Var> res;
        res.insert(property_var);
        return res;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        return os << std::string(' ', indent) << property_var;
    }
};
