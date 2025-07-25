#pragma once

#include <memory>
#include <vector>

#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace MQL {

class BindingExprAnd : public BindingExpr {
public:
    std::vector<std::unique_ptr<BindingExpr>> and_list;

    BindingExprAnd(std::vector<std::unique_ptr<BindingExpr>>&& and_list) :
        and_list(std::move(and_list))
    { }

    ObjectId eval(const Binding& binding) override
    {
        for (auto& expr : and_list) {
            auto oid = expr->eval(binding);

            if (oid == ObjectId(ObjectId::BOOL_TRUE)) {
                continue;
            } else if (oid == ObjectId(ObjectId::BOOL_FALSE)) {
                return oid;
            } else {
                return ObjectId::get_null();
            }
        }
        return ObjectId(ObjectId::BOOL_TRUE);
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    void print(std::ostream& os, std::vector<BindingIter*> ops) const override
    {
        os << '(';
        auto first = true;
        for (auto& expr : and_list) {
            if (first)
                first = false;
            else
                os << " AND ";
            expr->print(os, ops);
        }
        os << ')';
    }
};
} // namespace MQL
