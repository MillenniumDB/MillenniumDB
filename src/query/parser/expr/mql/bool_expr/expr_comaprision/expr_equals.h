#pragma once

#include <memory>

#include "query/parser/expr/expr.h"

namespace MQL {
class ExprEquals : public Expr {
public:
    std::unique_ptr<Expr> lhs;
    std::unique_ptr<Expr> rhs;

    ExprEquals(std::unique_ptr<Expr> lhs, std::unique_ptr<Expr> rhs) :
        lhs (std::move(lhs)),
        rhs (std::move(rhs)) { }

    virtual std::unique_ptr<Expr> clone() const override {
        return std::make_unique<ExprEquals>(lhs->clone(), rhs->clone());
    }

    void accept_visitor(ExprVisitor& visitor) override {
        visitor.visit(*this);
    }

    bool has_aggregation() const override {
        return lhs->has_aggregation() or rhs->has_aggregation();
    }

    std::set<VarId> get_all_vars() const override {
        std::set<VarId> res = lhs->get_all_vars();
        for (auto& var : rhs->get_all_vars()) {
            res.insert(var);
        }
        return res;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        return os << std::string(indent, ' ') << '(' <<*lhs << " == " << *rhs <<  ')' ;
    }
};
} // namespace MQL
