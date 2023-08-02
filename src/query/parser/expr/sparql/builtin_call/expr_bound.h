#pragma once

#include <memory>

#include "query/query_context.h"
#include "query/var_id.h"
#include "query/parser/expr/expr.h"

namespace SPARQL {
class ExprBound : public Expr {
public:
    VarId var;

    ExprBound(VarId var) :
        var (var) { }

    virtual std::unique_ptr<Expr> clone() const override {
        return std::make_unique<ExprBound>(var);
    }

    void accept_visitor(ExprVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override {
        return { var };
    }

    bool has_aggregation() const override {
        return false;
    }

    virtual std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        return os << std::string(indent, ' ') << "BOUND(?" << get_query_ctx().get_var_name(var) << ')';
    }
};
} // namespace SPARQL
