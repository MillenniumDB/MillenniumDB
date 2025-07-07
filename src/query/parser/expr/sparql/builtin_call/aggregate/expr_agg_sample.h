#pragma once

#include <memory>

#include "query/parser/expr/sparql/expr.h"

namespace SPARQL {

/* https://www.w3.org/TR/sparql11-query/#defn_aggSample
Definition: Sample
RDFTerm Sample(multiset M)

Sample(M) = v, where v in Flatten(M)

Sample({}) = error
*/

// Sample is a set function which returns an arbitrary value from the multiset
// passed to it.
class ExprAggSample : public Expr {
public:
    std::unique_ptr<Expr> expr;
    bool distinct;

    ExprAggSample(std::unique_ptr<Expr> expr, bool distinct) :
        expr     (std::move(expr)),
        distinct (distinct) { }

    virtual std::unique_ptr<Expr> clone() const override {
        return std::make_unique<ExprAggSample>(expr->clone(), distinct);
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
