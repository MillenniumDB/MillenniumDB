#pragma once

#include <memory>
#include <vector>

#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace GQL {

class BindingExprAnd : public BindingExpr {
public:
    std::vector<std::unique_ptr<BindingExpr>> and_list;

    BindingExprAnd(std::vector<std::unique_ptr<BindingExpr>>&& and_list) :
        and_list(std::move(and_list))
    { }

    ObjectId eval(const Binding& binding) override
    {
        // three-valued: false if an operand is false, otherwise null if an operand is not true
        bool null_seen = false;
        for (auto& expr : and_list) {
            auto oid = expr->eval(binding);

            if (oid == ObjectId(ObjectId::BOOL_FALSE)) {
                return oid;
            } else if (oid != ObjectId(ObjectId::BOOL_TRUE)) {
                null_seen = true;
            }
        }
        return null_seen ? ObjectId::get_null() : ObjectId(ObjectId::BOOL_TRUE);
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    void print(std::ostream& os, std::vector<BindingIter*>& ops) const override
    {
        os << '(';
        auto first = true;
        for (auto& e : and_list) {
            if (first) {
                first = false;
            } else {
                os << " AND ";
            }
            e->print(os, ops);
        }
        os << ")";
    }
};
} // namespace GQL
