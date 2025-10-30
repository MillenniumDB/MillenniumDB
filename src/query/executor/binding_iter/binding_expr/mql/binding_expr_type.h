#pragma once

#include "graph_models/quad_model/quad_model.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace MQL {
class BindingExprType : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    BindingExprType(std::unique_ptr<BindingExpr> expr_) :
        expr(std::move(expr_))
    { }

    ObjectId eval(const Binding& binding) override
    {
        const ObjectId oid = expr->eval(binding);

        if (oid.get_type() != ObjectId::MASK_EDGE) {
            return ObjectId::get_null();
        }

        bool interruption = false;
        BptIter<4> iter = quad_model.edge_from_to_type->get_range(
            &interruption,
            { oid.id, 0, 0, 0 },
            { oid.id, UINT64_MAX, UINT64_MAX, UINT64_MAX }
        );

        auto record = iter.next();
        return record == nullptr ? ObjectId::get_null() : ObjectId((*record)[3]);
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    void print(std::ostream& os, std::vector<BindingIter*>& ops) const override
    {
        os << "TYPE(";
        expr->print(os, ops);
        os << ')';
    }
};
} // namespace MQL
