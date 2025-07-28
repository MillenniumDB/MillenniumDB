#pragma once

#include "graph_models/gql/conversions.h"
#include "graph_models/gql/gql_model.h"
#include "graph_models/object_id.h"
#include "query/executor/binding.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"
#include "query/query_context.h"
#include "query/rewriter/gql/op/var_type.h"
#include "query/var_id.h"
#include "storage/index/bplus_tree/bplus_tree.h"

namespace GQL {

class BindingExprWildcardLabel : public BindingExpr {
public:
    VarId var_id;
    VarType::Type type;

    BindingExprWildcardLabel(VarId var_id, VarType::Type type) :
        var_id(var_id),
        type(type)
    { }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    ObjectId eval(const Binding& binding) override
    {
        uint64_t object_id = binding[var_id].id;

        bool interruption = false;
        BptIter<2> it;
        if (type == VarType::Node) {
            it = gql_model.node_label->get_range(&interruption, { object_id, 0 }, { object_id, UINT64_MAX });
        } else {
            it = gql_model.edge_label->get_range(&interruption, { object_id, 0 }, { object_id, UINT64_MAX });
        }

        if (it.next() != nullptr) {
            return GQL::Conversions::pack_bool(true);
        }
        return GQL::Conversions::pack_bool(false);
    }

    void print(std::ostream& os, std::vector<BindingIter*>) const override
    {
        os << "HasWildcardLabel(" << var_id << ')';
    }
};
} // namespace GQL
