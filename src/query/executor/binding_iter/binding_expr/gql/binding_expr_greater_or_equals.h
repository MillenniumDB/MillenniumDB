#pragma once

#include <memory>

#include "graph_models/gql/comparisons.h"
#include "graph_models/gql/conversions.h"
#include "graph_models/object_id.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace GQL {
class BindingExprGreaterOrEquals : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> lhs;
    std::unique_ptr<BindingExpr> rhs;

    BindingExprGreaterOrEquals(std::unique_ptr<BindingExpr> lhs, std::unique_ptr<BindingExpr> rhs) :
        lhs(std::move(lhs)),
        rhs(std::move(rhs))
    { }

    ObjectId eval(const Binding& binding) override
    {
        auto lhs_oid = lhs->eval(binding);
        auto rhs_oid = rhs->eval(binding);

        bool error;
        bool res = GQL::Comparisons::strict_compare_null_first(lhs_oid, rhs_oid, &error) >= 0;
        if (error) {
            return ObjectId::get_null();
        }
        return GQL::Conversions::pack_bool(res);
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    void print(std::ostream& os, std::vector<BindingIter*> ops) const override
    {
        os << '(';
        lhs->print(os, ops);
        os << " >= ";
        rhs->print(os, ops);
        os << ')';
    }
};
} // namespace GQL
