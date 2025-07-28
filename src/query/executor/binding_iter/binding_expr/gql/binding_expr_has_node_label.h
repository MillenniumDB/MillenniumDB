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

class BindingExprHasNodeLabel : public BindingExpr {
public:
    VarId node_id;
    ObjectId label_id;

    BindingExprHasNodeLabel(VarId node_id, ObjectId label_id) :
        node_id(node_id),
        label_id(label_id)
    { }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    ObjectId eval(const Binding& binding) override
    {
        bool interruption = false;
        ObjectId node_oid = binding[node_id];

        BptIter<2> it = gql_model.node_label->get_range(
            &interruption,
            { node_oid.id, label_id.id },
            { node_oid.id, label_id.id }
        );

        if (it.next()) {
            return GQL::Conversions::pack_bool(true);
        }
        return GQL::Conversions::pack_bool(false);
    }

    void print(std::ostream& os, std::vector<BindingIter*>) const override
    {
        os << "HasNodeLabel(";
        os << node_id << ", " << label_id;
        os << ')';
    }
};
} // namespace GQL
