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
        /* this assumes the following rules:
         *     | T | F | ? |
         * | T | T | F | ? |
         * | F | F | F | F |
         * | ? | ? | F | ? |
         */

        bool null_seen = false;
        for (auto& expr : and_list) {
            const auto oid = MQL::Conversions::to_boolean(expr->eval(binding));

            if (oid == ObjectId(ObjectId::BOOL_FALSE)) {
                return ObjectId(ObjectId::BOOL_FALSE);
            } else if (oid == ObjectId::get_null()) {
                null_seen = true;
            }
        }
        return null_seen ? ObjectId::get_null() : ObjectId(ObjectId::BOOL_TRUE);
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
