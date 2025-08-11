#pragma once

#include "graph_models/gql/conversions.h"
#include "graph_models/gql/gql_model.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"
#include "query/query_context.h"
#include "storage/index/bplus_tree/bplus_tree.h"

namespace GQL {
class BindingExprNodeLabels : public BindingExpr {
public:
    VarId object_var;

    BindingExprNodeLabels(VarId object) :
        object_var(object)
    { }

    ObjectId eval(const Binding& binding) override
    {
        ObjectId object_id = binding[object_var];

        bool interruption = false;
        BptIter<2> it = gql_model.node_label
                            ->get_range(&interruption, { object_id.id, 0 }, { object_id.id, UINT64_MAX });

        std::vector<ObjectId> labels;

        auto record = it.next();
        while (record != nullptr) {
            ObjectId label_oid((*record)[1]);
            labels.push_back(label_oid);
            record = it.next();
        }
        ObjectId list_oid = Conversions::pack_list(labels);
        return list_oid;
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    void print(std::ostream& os, std::vector<BindingIter*>&) const override
    {
        os << "Labels(" << object_var << ")\n";
    }
};
} // namespace GQL
