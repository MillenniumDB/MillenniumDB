#pragma once

#include "graph_models/quad_model/conversions.h"
#include "graph_models/quad_model/quad_model.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace MQL {
class BindingExprLabels : public BindingExpr {
public:
    VarId var;

    BindingExprLabels(VarId var) :
        var(var)
    { }

    ObjectId eval(const Binding& binding) override
    {
        ObjectId node = binding[var];

        if ((node.id & ObjectId::GENERIC_TYPE_MASK) != ObjectId::MASK_NAMED_NODE) {
            return ObjectId::get_null();
        }

        bool interruption = false;
        BptIter<2> iter = quad_model.node_label
                              ->get_range(&interruption, { node.id, 0 }, { node.id, UINT64_MAX });

        std::vector<ObjectId> labels;

        auto record = iter.next();

        while (record != nullptr) {
            labels.emplace_back((*record)[1]);
            record = iter.next();
        }
        return Conversions::pack_list(labels);
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    void print(std::ostream& os, std::vector<BindingIter*>&) const override
    {
        os << "LABELS(" << var << ')';
    }
};
} // namespace MQL
