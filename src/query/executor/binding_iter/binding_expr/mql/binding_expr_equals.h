#pragma once

#include <memory>

#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace MQL {

class BindingExprEquals : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> lhs;
    std::unique_ptr<BindingExpr> rhs;

    BindingExprEquals(std::unique_ptr<BindingExpr> lhs, std::unique_ptr<BindingExpr> rhs) :
        lhs (std::move(lhs)),
        rhs (std::move(rhs)) { }


    ObjectId eval(const Binding& binding) override {
        auto lhs_oid = lhs->eval(binding);
        auto rhs_oid = rhs->eval(binding);

        // Check if the ids are equal
        if (lhs_oid == rhs_oid) {
            return ObjectId(ObjectId::BOOL_TRUE);
        } else {
            return ObjectId(ObjectId::BOOL_FALSE);
        }
    }

    std::ostream& print_to_ostream(std::ostream& os) const override {
        os << '(' << *lhs << '=' << *rhs << ')';
        return os;
    }
};
} // namespace MQL
