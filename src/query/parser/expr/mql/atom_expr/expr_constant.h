#pragma once

#include "graph_models/object_id.h"
#include "query/query_context.h"
#include "query/parser/expr/expr.h"

namespace MQL {
class ExprConstant : public Expr {
public:
    ObjectId value;

    ExprConstant(ObjectId value) :
        value (value) { }

    virtual std::unique_ptr<Expr> clone() const override {
        return std::make_unique<ExprConstant>(value);
    }

    void accept_visitor(ExprVisitor& visitor) override {
        visitor.visit(*this);
    }

    bool has_aggregation() const override { return false; }

    std::set<VarId> get_all_vars() const override {
        return { };
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        return os << std::string(indent, ' ') << value;
    }
};
} // namespace MQL
