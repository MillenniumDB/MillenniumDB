#pragma once

#include <memory>

#include "graph_models/object_id.h"
#include "graph_models/quad_model/comparisons.h"
#include "graph_models/quad_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace MQL {
class BindingExprLessOrEquals : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> lhs;
    std::unique_ptr<BindingExpr> rhs;

    BindingExprLessOrEquals(std::unique_ptr<BindingExpr> lhs, std::unique_ptr<BindingExpr> rhs) :
        lhs(std::move(lhs)),
        rhs(std::move(rhs))
    { }

    ObjectId eval(const Binding& binding) override
    {
        auto lhs_oid = lhs->eval(binding);
        auto rhs_oid = rhs->eval(binding);

        bool res = MQL::Comparisons::compare(lhs_oid, rhs_oid) <= 0;

        return MQL::Conversions::pack_bool(res);
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    void print(std::ostream& os, std::vector<BindingIter*> ops) const override
    {
        os << "(";
        lhs->print(os, ops);
        os << " <= ";
        rhs->print(os, ops);
        os << ")";
    }
};
} // namespace MQL
