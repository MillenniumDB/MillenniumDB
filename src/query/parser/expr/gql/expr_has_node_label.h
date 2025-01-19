#pragma once

#include <memory>

#include "graph_models/object_id.h"
#include "query/parser/expr/expr.h"

namespace GQL {
class ExprHasNodeLabel : public Expr {
public:
    VarId node_id;
    ObjectId label_id;

    ExprHasNodeLabel(VarId node_id, ObjectId label_id) :
        node_id(node_id),
        label_id(label_id)
    { }

    virtual std::unique_ptr<Expr> clone() const override
    {
        return std::make_unique<ExprHasNodeLabel>(*this);
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
        return { node_id };
    }
};
} // namespace GQL
