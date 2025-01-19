#pragma once

#include "graph_models/object_id.h"
#include "query/parser/expr/expr.h"

namespace MQL {
class ExprVarProperty : public Expr {
public:
    VarId var_without_property; // ?x

    ObjectId key;

    VarId var_with_property; // ?x.key

    ExprVarProperty(VarId var_without_property, ObjectId key, VarId var_with_property) :
        var_without_property (var_without_property),
        key                  (key),
        var_with_property    (var_with_property) { }

    virtual std::unique_ptr<Expr> clone() const override {
        return std::make_unique<ExprVarProperty>(
            var_without_property,
            key,
            var_with_property
        );
    }

    void accept_visitor(ExprVisitor& visitor) override {
        visitor.visit(*this);
    }

    bool has_aggregation() const override { return false; }

    std::set<VarId> get_all_vars() const override {
        return { var_with_property };
    }
};
} // namespace MQL
