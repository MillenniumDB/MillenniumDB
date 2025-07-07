#pragma once

#include <memory>

#include "query/parser/expr/sparql/expr.h"

namespace SPARQL {

class ExprReplace : public Expr {
public:
    std::unique_ptr<Expr> expr1;
    std::unique_ptr<Expr> expr2;
    std::unique_ptr<Expr> expr3;

    // may be nullptr
    std::unique_ptr<Expr> expr4;

    ExprReplace(std::unique_ptr<Expr> expr1,
                std::unique_ptr<Expr> expr2,
                std::unique_ptr<Expr> expr3) :
        expr1 (std::move(expr1)),
        expr2 (std::move(expr2)),
        expr3 (std::move(expr3)) { }

    ExprReplace(std::unique_ptr<Expr> expr1,
                std::unique_ptr<Expr> expr2,
                std::unique_ptr<Expr> expr3,
                std::unique_ptr<Expr> expr4) :
        expr1 (std::move(expr1)),
        expr2 (std::move(expr2)),
        expr3 (std::move(expr3)),
        expr4 (std::move(expr4)) { }

    virtual std::unique_ptr<Expr> clone() const override {
        return std::make_unique<ExprReplace>(expr1->clone(),
                                             expr2->clone(),
                                             expr3->clone());
    }

    void accept_visitor(ExprVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override {
        std::set<VarId> res;
        auto expr1_vars = expr1->get_all_vars();
        auto expr2_vars = expr2->get_all_vars();
        auto expr3_vars = expr3->get_all_vars();
        res.insert(expr1_vars.begin(), expr1_vars.end());
        res.insert(expr2_vars.begin(), expr2_vars.end());
        res.insert(expr3_vars.begin(), expr3_vars.end());

        if (expr4 != nullptr) {
            auto expr4_vars = expr4->get_all_vars();
            res.insert(expr4_vars.begin(), expr4_vars.end());
        }
        return res;
    }

    bool has_aggregation() const override {
        return expr1->has_aggregation() || expr2->has_aggregation() || expr3->has_aggregation() || (expr4 && expr4->has_aggregation());
    }
};
} // namespace SPARQL
