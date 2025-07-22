#pragma once

#include "graph_models/gql/conversions.h"
#include "graph_models/gql/gql_model.h"
#include "graph_models/object_id.h"
#include "query/executor/binding.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"
#include "query/query_context.h"
#include "query/var_id.h"
#include "storage/index/bplus_tree/bplus_tree.h"

namespace GQL {

class BindingExprHasEdgeLabel : public BindingExpr {
public:
    VarId edge_id;
    ObjectId label_id;

    BindingExprHasEdgeLabel(VarId edge_id, ObjectId label_id) :
        edge_id(edge_id),
        label_id(label_id)
    { }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    ObjectId eval(const Binding& binding) override
    {
        bool interruption = false;
        ObjectId edge_oid = binding[edge_id];

        BptIter<2> it = gql_model.edge_label->get_range(
            &interruption,
            { edge_oid.id, label_id.id },
            { edge_oid.id, label_id.id }
        );

        if (it.next()) {
            return GQL::Conversions::pack_bool(true);
        }
        return GQL::Conversions::pack_bool(false);
    }

    void print(std::ostream& os, std::vector<BindingIter*>) const override
    {
        os << "HasEdgeLabel(";
        os << edge_id << ", " << label_id;
        os << ')';
    }
};
} // namespace GQL
