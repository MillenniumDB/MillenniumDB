#pragma once

#include "graph_models/object_id.h"
#include "query/parser/expr/gql/expr.h"

namespace GQL {
class ExprHasEdgeLabel : public Expr {
public:
    VarId edge_id;
    ObjectId label_id;

    ExprHasEdgeLabel(VarId edge_id, ObjectId label_id) :
        edge_id(edge_id),
        label_id(label_id)
    { }

    virtual std::unique_ptr<Expr> clone() const override
    {
        return std::make_unique<ExprHasEdgeLabel>(*this);
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
        return { edge_id };
    }
};
} // namespace GQL
