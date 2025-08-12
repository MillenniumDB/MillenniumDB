#pragma once

#include <memory>
#include <vector>

#include "graph_models/quad_model/conversions.h"
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
        /* this assumes the following rules:
         *     | T | F | ? |
         * | T | T | T | T |
         * | F | T | F | ? |
         * | ? | T | ? | ? |
         */

        bool null_seen = false;
        for (auto& expr : or_list) {
            const auto oid = MQL::Conversions::to_boolean(expr->eval(binding));

            if (oid == ObjectId(ObjectId::BOOL_TRUE)) {
                return ObjectId(ObjectId::BOOL_TRUE);
            } else if (oid == ObjectId::get_null()) {
                null_seen = true;
            }
        }
        return null_seen ? ObjectId::get_null() : ObjectId(ObjectId::BOOL_FALSE);
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
