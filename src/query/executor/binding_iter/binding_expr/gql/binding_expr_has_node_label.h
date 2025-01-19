#pragma once

#include "graph_models/gql/conversions.h"
#include "graph_models/object_id.h"
#include "query/executor/binding.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"
#include "query/query_context.h"
#include "query/var_id.h"
#include "storage/index/lists/tmp_lists.h"

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
        std::string var_name = "." + get_query_ctx().get_var_name(node_id) + "_labels";
        bool found;
        VarId labels_var = get_query_ctx().get_var(var_name, &found);

        std::vector<ObjectId> labels;
        ObjectId list_oid = binding[labels_var];
        Conversions::unpack_list(list_oid, labels);

        auto pos = std::find(labels.begin(), labels.end(), label_id);

        if (pos != labels.end()) {
            return GQL::Conversions::pack_bool(true);
        }
        return GQL::Conversions::pack_bool(false);
    }
};
} // namespace GQL
