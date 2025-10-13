#pragma once

#include "graph_models/gql/comparisons.h"
#include "graph_models/gql/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace GQL {
class BindingExprIn : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> lhs;
    std::unique_ptr<BindingExpr> rhs;

    BindingExprIn(std::unique_ptr<BindingExpr> lhs, std::unique_ptr<BindingExpr> rhs) :
        lhs(std::move(lhs)),
        rhs(std::move(rhs))
    { }

    ObjectId eval(const Binding& binding) override
    {
        ObjectId list_id = rhs->eval(binding);
        if (GQL_OID::get_type(list_id) != GQL_OID::Type::LIST) {
            return ObjectId::get_null();
        }
        std::vector<ObjectId> list = Conversions::unpack_list(list_id);

        ObjectId elem = lhs->eval(binding);

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
        lhs->print(os, ops);
        os << " IN ";
        rhs->print(os, ops);
        os << ')';
    }
};
} // namespace GQL
