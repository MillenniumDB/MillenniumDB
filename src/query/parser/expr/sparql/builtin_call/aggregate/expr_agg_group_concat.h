#pragma once

#include <memory>

#include "query/parser/expr/expr.h"

namespace SPARQL {

// If the "separator" scalar argument is absent from GROUP_CONCAT
// then it is taken to be the "space" character, unicode codepoint U+0020.
class ExprAggGroupConcat : public Expr {
public:
    std::unique_ptr<Expr> expr;
    std::string separator;
    bool distinct;

    ExprAggGroupConcat(std::unique_ptr<Expr> expr, std::string&& separator, bool distinct) :
        expr      (std::move(expr)),
        separator (std::move(separator)),
        distinct  (distinct) { }

    virtual std::unique_ptr<Expr> clone() const override {
        return std::make_unique<ExprAggGroupConcat>(expr->clone(), separator, distinct);
    }

    ExprAggGroupConcat(std::unique_ptr<Expr> expr, const std::string& separator, bool distinct) :
        expr      (std::move(expr)),
        separator (separator),
        distinct  (distinct) { }

    void accept_visitor(ExprVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override {
        return expr->get_all_vars();
    }

    bool has_aggregation() const override {
        return true;
    }

    virtual std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        return os << std::string(indent, ' ') << "GROUP_CONCAT(" << (distinct ? "DISTINCT " : "") << *expr << ", separator=" << separator << ")";
    }
};
} // namespace SPARQL
