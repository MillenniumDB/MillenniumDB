#pragma once

#include <memory>

#include "query/parser/expr/expr.h"

namespace SPARQL {
class ExprIRI : public Expr {
public:
    std::unique_ptr<Expr> expr;
    std::string base_iri;

    ExprIRI(std::unique_ptr<Expr> expr, std::string base_iri) :
        expr (std::move(expr)), base_iri (base_iri) { }

    virtual std::unique_ptr<Expr> clone() const override {
        return std::make_unique<ExprIRI>(expr->clone(), base_iri);
    }

    void accept_visitor(ExprVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override {
        return expr->get_all_vars();
    }

    bool has_aggregation() const override {
        return expr->has_aggregation();
    }

    virtual std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        return os << std::string(indent, ' ') << "IRI(" << *expr << ')';
    }
};
} // namespace SPARQL
