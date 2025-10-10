#pragma once

#include "graph_models/gql/comparisons.h"
#include "graph_models/gql/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace GQL {
class BindingExprIn : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;
    ObjectId list_id;

    BindingExprIn(std::unique_ptr<BindingExpr> expr, ObjectId list_id) :
        expr(std::move(expr)),
        list_id(list_id)
    { }

    ObjectId eval(const Binding& binding) override
    {
        std::vector<ObjectId> list = Conversions::unpack_list(list_id);

        ObjectId elem = expr->eval(binding);

        for (auto& oid : list) {
            if (GQL::Comparisons::compare_null_last(elem, oid) == 0) {
                return ObjectId(ObjectId::BOOL_TRUE);
            }
        }
        return ObjectId(ObjectId::BOOL_FALSE);
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    void print(std::ostream& os, std::vector<BindingIter*>& ops) const override
    {
        os << '(';
        expr->print(os, ops);
        os << " IN " << list_id << ')';
    }
};
} // namespace GQL
