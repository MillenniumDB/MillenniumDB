#pragma once

#include "graph_models/object_id.h"
#include "query/parser/expr/mql/expr.h"

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

    bool operator<(const ExprVarProperty& other) const {
        if (var_without_property < other.var_without_property) {
            return true;
        } else if (other.var_without_property < var_without_property) {
            return false;
        }

        if (key < other.key) {
            return true;
        } else if (other.key < key) {
            return false;
        }

        if (var_with_property < other.var_with_property) {
            return true;
        } else if (other.var_with_property < var_with_property) {
            return false;
        }

        return false;
    }
};
} // namespace MQL
