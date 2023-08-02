#pragma once

#include "query/var_id.h"
#include "query/query_context.h"
#include "query/parser/expr/expr.h"

namespace SPARQL {
class ExprVar : public Expr {
public:
    VarId var;

    ExprVar(VarId var) : var (var) { }

    virtual std::unique_ptr<Expr> clone() const override {
        return std::make_unique<ExprVar>(var);
    }

    void accept_visitor(ExprVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override {
        std::set<VarId> res;
        res.insert(var);
        return res;
    }

    bool has_aggregation() const override {
        return false;
    }

    virtual std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        return os << '?' << std::string(indent, ' ') << get_query_ctx().get_var_name(var);
    }
};
} // namespace SPARQL
