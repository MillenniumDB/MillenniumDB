#pragma once

#include "query/var_id.h"
#include "query/query_context.h"
#include "query/parser/expr/sparql/expr.h"

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
};
} // namespace SPARQL
