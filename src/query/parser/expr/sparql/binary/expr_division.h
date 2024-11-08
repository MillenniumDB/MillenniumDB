#pragma once

#include <memory>

#include "query/parser/expr/expr.h"

namespace SPARQL {
class ExprDivision : public Expr {
public:
    std::unique_ptr<Expr> lhs;
    std::unique_ptr<Expr> rhs;

    ExprDivision(std::unique_ptr<Expr> lhs, std::unique_ptr<Expr> rhs) :
        lhs (std::move(lhs)),
        rhs (std::move(rhs)) { }

    virtual std::unique_ptr<Expr> clone() const override {
        return std::make_unique<ExprDivision>(lhs->clone(), rhs->clone());
    }

    void accept_visitor(ExprVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override {
        auto res = lhs->get_all_vars();
        auto rhs_vars = rhs->get_all_vars();
        res.insert(rhs_vars.begin(), rhs_vars.end());
        return res;
    }

    bool has_aggregation() const override {
        return lhs->has_aggregation() || rhs->has_aggregation();
    }
};
} // namespace SPARQL
