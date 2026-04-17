#pragma once

#include "graph_models/quad_model/conversions.h"
#include "graph_models/quad_model/quad_model.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace MQL {
class BindingExprLabels : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    BindingExprLabels(std::unique_ptr<BindingExpr> expr_) :
        expr(std::move(expr_))
    { }

    ObjectId eval(const Binding& binding) override
    {
        const ObjectId oid = expr->eval(binding);

        if (oid.generic_type() != ObjectGenType::NamedNode) {
            return ObjectId::get_null();
        }

        bool interruption = false;
        auto iter = quad_model.node_label->get_range(&interruption, { oid.id, 0 }, { oid.id, UINT64_MAX });

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
        os << "LABELS(";
        expr->print(os, ops);
        os << ')';
    }
};
} // namespace MQL
