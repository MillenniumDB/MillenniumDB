#pragma once

#include <memory>

#include "query/parser/expr/expr.h"

namespace SPARQL {
class ExprUUID : public Expr {
public:

    ExprUUID() = default;

    virtual std::unique_ptr<Expr> clone() const override {
        return std::make_unique<ExprUUID>();
    }

    void accept_visitor(ExprVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override {
        return {};
    }

    bool has_aggregation() const override { return false; }

    virtual std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        return os << std::string(indent, ' ') << "UUID()";
    }
};
} // namespace SPARQL
