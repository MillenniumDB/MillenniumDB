#pragma once

#include <memory>
#include <vector>

#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace MQL {

class BindingExprOr : public BindingExpr {
public:
    std::vector<std::unique_ptr<BindingExpr>> or_list;

    BindingExprOr(std::vector<std::unique_ptr<BindingExpr>>&& or_list) :
        or_list(std::move(or_list))
    { }

    ObjectId eval(const Binding& binding) override
    {
        for (auto& expr : or_list) {
            auto oid = expr->eval(binding);

            if (oid == ObjectId(ObjectId::BOOL_FALSE)) {
                continue;
            } else if (oid == ObjectId(ObjectId::BOOL_TRUE)) {
                return oid;
            } else {
                return ObjectId::get_null();
            }
        }
        return ObjectId(ObjectId::BOOL_FALSE);
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    void print(std::ostream& os, std::vector<BindingIter*> ops) const override
    {
        os << '(';
        auto first = true;
        for (auto& expr : or_list) {
            if (first)
                first = false;
            else
                os << " OR ";
            expr->print(os, ops);
        }
        os << ')';
    }
};
} // namespace MQL
