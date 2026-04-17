#pragma once

#include "query/executor/binding_iter/binding_expr/binding_expr.h"

#include <memory>

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

        if (lhs_oid.type() != ObjectType::Bool || rhs_oid.type() != ObjectType::Bool) {
            return ObjectId::get_null();
        }
        bool res = (lhs_oid.is_true() && rhs_oid.is_false()) || (lhs_oid.is_false() && rhs_oid.is_true());

        return ObjectId(static_cast<uint64_t>(res));
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    void print(std::ostream& os, std::vector<BindingIter*>& ops) const override
    {
        os << '(';
        lhs->print(os, ops);
        os << " XOR ";
        rhs->print(os, ops);
        os << ')';
    }
};
} // namespace GQL
