#pragma once

#include "graph_models/object_id.h"
#include "query/query_context.h"
#include "query/parser/expr/expr.h"

namespace SPARQL {
class ExprTerm : public Expr {
public:
    ObjectId term;

    ExprTerm(ObjectId term) : term (term) { }

    virtual std::unique_ptr<Expr> clone() const override {
        return std::make_unique<ExprTerm>(term);
    }

    void accept_visitor(ExprVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override {
        return { };
    }

    bool has_aggregation() const override {
        return false;
    }

    virtual std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        return os << std::string(indent, ' ') << term;
    }
};
} // namespace SPARQL
