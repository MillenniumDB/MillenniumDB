#pragma once

#include <memory>
#include <vector>

#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace GQL {

class BindingExprOr : public BindingExpr {
public:
    std::vector<std::unique_ptr<BindingExpr>> or_list;

    BindingExprOr(std::vector<std::unique_ptr<BindingExpr>>&& or_list) :
        or_list(std::move(or_list))
    { }

    ObjectId eval(const Binding& binding) override
    {
        // three-valued: true if an operand is true, otherwise null if an operand is not false
        bool null_seen = false;
        for (auto& expr : or_list) {
            auto oid = expr->eval(binding);

            if (oid == ObjectId(ObjectId::BOOL_TRUE)) {
                return oid;
            } else if (oid != ObjectId(ObjectId::BOOL_FALSE)) {
                null_seen = true;
            }
        }
        return null_seen ? ObjectId::get_null() : ObjectId(ObjectId::BOOL_FALSE);
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    void print(std::ostream& os, std::vector<BindingIter*>& ops) const override
    {
        os << '(';
        auto first = true;
        for (auto& e : or_list) {
            if (first) {
                first = false;
            } else {
                os << " OR ";
            }
            e->print(os, ops);
        }
        os << ")";
    }
};
} // namespace GQL
