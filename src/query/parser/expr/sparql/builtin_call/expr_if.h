#pragma once

#include <memory>

#include "query/parser/expr/expr.h"

namespace SPARQL {

/*
rdfTerm  IF (expression1, expression2, expression3)

The IF function form evaluates the first argument, interprets it as a effective
boolean value, then returns the value of expression2 if the EBV is true,
otherwise it returns the value of expression3. Only one of expression2 and
expression3 is evaluated. If evaluating the first argument raises an error,
then an error is raised for the evaluation of the IF expression.

Examples: Suppose ?x = 2, ?z = 0 and ?y is not bound in some query solution:
IF(?x = 2, "yes", "no")	returns "yes"
IF(bound(?y), "yes", "no")	returns "no"
IF(?x=2, "yes", 1/?z)	returns "yes", the expression 1/?z is not evaluated
IF(?x=1, "yes", 1/?z)	raises an error
IF("2" > 1, "yes", "no")	raises an error
*/
class ExprIf : public Expr {
public:
    std::unique_ptr<Expr> expr1;
    std::unique_ptr<Expr> expr2;
    std::unique_ptr<Expr> expr3;

    ExprIf(std::unique_ptr<Expr> expr1,
           std::unique_ptr<Expr> expr2,
           std::unique_ptr<Expr> expr3) :
        expr1 (std::move(expr1)),
        expr2 (std::move(expr2)),
        expr3 (std::move(expr3)) { }

    virtual std::unique_ptr<Expr> clone() const override {
        return std::make_unique<ExprIf>(
                expr1->clone(), expr2->clone(), expr3->clone());
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
        return res;
    }

    bool has_aggregation() const override {
        return expr1->has_aggregation() or expr2->has_aggregation() or expr3->has_aggregation();
    }

    virtual std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        return os << std::string(indent, ' ') << "IF("
                  << *expr1 << ", "  << *expr2 << ", " << *expr3 << ')';
    }
};
} // namespace SPARQL
