#pragma once

#include <memory>

#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace MQL {

class BindingExprNotEquals : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> lhs;
    std::unique_ptr<BindingExpr> rhs;

    BindingExprNotEquals(std::unique_ptr<BindingExpr> lhs, std::unique_ptr<BindingExpr> rhs) :
        lhs (std::move(lhs)),
        rhs (std::move(rhs)) { }


    ObjectId eval(const Binding& binding) override {
        auto lhs_oid = lhs->eval(binding);
        auto rhs_oid = rhs->eval(binding);

        // Check if the ids are equal
        if (lhs_oid != rhs_oid) {
            return ObjectId(ObjectId::BOOL_TRUE);
        } else {
            return ObjectId(ObjectId::BOOL_FALSE);
        }
    }

    void accept_visitor(BindingExprVisitor& visitor) override {
        visitor.visit(*this);
    }
};
} // namespace MQL
