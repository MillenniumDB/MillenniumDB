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
        bool interruption = false;
        ObjectId node = binding[var];
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

    void print(std::ostream& os, std::vector<BindingIter*>& ops) const override
    {
        os << "LABELS(" << var << ')';
    }
};
} // namespace MQL
