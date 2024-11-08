#pragma once

#include <memory>

#include "query/parser/expr/expr.h"

namespace SPARQL {

/* https://www.w3.org/TR/sparql11-query/#defn_aggSum
Sum is a SPARQL set function that will return the numeric value obtained by
summing the values within the aggregate group. Type promotion happens as per
the op:numeric-add function, applied transitively, (see definition below) so
the value of SUM(?x), in an aggregate group where ?x has values 1 (integer),
2.0e0 (float), and 3.0 (decimal) will be 6.0 (float).

Definition: Sum
numeric Sum(multiset M)

The Sum set function is used by the SUM aggregate in the syntax.

Sum(M) = Sum(ToList(Flatten(M))).

Sum(S) = op:numeric-add(S1, Sum(S2..n)) when card[S] > 1
Sum(S) = op:numeric-add(S1, 0) when card[S] = 1
Sum(S) = "0"^^xsd:integer when card[S] = 0

In this way, Sum({1, 2, 3}) = op:numeric-add(1, op:numeric-add(2, op:numeric-add(3, 0))).

*/
class ExprAggSum : public Expr {
public:
    std::unique_ptr<Expr> expr;
    bool distinct;

    ExprAggSum(std::unique_ptr<Expr> expr, bool distinct) :
        expr     (std::move(expr)),
        distinct (distinct) { }

    virtual std::unique_ptr<Expr> clone() const override {
        return std::make_unique<ExprAggSum>(expr->clone(), distinct);
    }

    void accept_visitor(ExprVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override {
        return expr->get_all_vars();
    }

    bool has_aggregation() const override {
        return true;
    }
};
} // namespace SPARQL
