#pragma once

#include "graph_models/object_id.h"
#include "query/parser/expr/gql/expr.h"
#include "query/rewriter/gql/op/var_type.h"

namespace GQL {
class ExprProperty : public Expr {
public:
    VarId object;
    ObjectId key;
    VarId value;
    VarType::Type type;

    ExprProperty(VarId object, ObjectId key, VarId value, VarType::Type type) :
        object(object),
        key(key),
        value(value),
        type(type)
    { }

    virtual std::unique_ptr<Expr> clone() const override
    {
        return std::make_unique<ExprProperty>(*this);
    }

    void accept_visitor(ExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    bool has_aggregation() const override
    {
        return false;
    }

    std::set<VarId> get_all_vars() const override
    {
        // only returns the var with the property
        return { value };
    }

    bool operator<(const ExprProperty& other) const
    {
        if (type != other.type) {
            return type < other.type;

        } else if (object != other.object) {
            return object < other.object;

        } else if (key != other.key) {
            return key < other.key;
        }
        return value < other.value;
    }
};
} // namespace GQL
