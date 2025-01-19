#pragma once

#include <memory>

#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace GQL {

class BindingExprXor : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> lhs;
    std::unique_ptr<BindingExpr> rhs;

    BindingExprXor(std::unique_ptr<BindingExpr> lhs, std::unique_ptr<BindingExpr> rhs) :
        lhs(std::move(lhs)),
        rhs(std::move(rhs))
    { }

    ObjectId eval(const Binding& binding) override
    {
        ObjectId lhs_oid = lhs->eval(binding);
        ObjectId rhs_oid = rhs->eval(binding);

        if ((lhs_oid.id & ObjectId::TYPE_MASK) != ObjectId::MASK_BOOL
            || (rhs_oid.id & ObjectId::TYPE_MASK) != ObjectId::MASK_BOOL)
        {
            return ObjectId::get_null();
        }

        if ((lhs_oid.is_true() && rhs_oid.is_false()) || (lhs_oid.is_false() && rhs_oid.is_true())) {
            return ObjectId(ObjectId::BOOL_TRUE);
        }

        return ObjectId(ObjectId::BOOL_FALSE);
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }
};
} // namespace GQL
