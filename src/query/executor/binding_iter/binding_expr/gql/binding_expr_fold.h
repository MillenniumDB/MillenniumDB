#pragma once

#include <cassert>
#include <memory>

#include "graph_models/gql/conversions.h"
#include "misc/transliterator.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace GQL {
class BindingExprFold : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;
    bool upper;

    BindingExprFold(std::unique_ptr<BindingExpr> expr, bool upper) :
        expr(std::move(expr)),
        upper(std::move(upper))
    { }

    ObjectId eval(const Binding& binding) override
    {
        auto expr_oid = expr->eval(binding);
        auto expr_generic_type = GQL_OID::get_generic_type(expr_oid);

        if (expr_generic_type == GQL_OID::GenericType::STRING) {
            auto str = GQL::Conversions::unpack_string(expr_oid);
            if (upper) {
                return Conversions::pack_string_simple(Transliterator::uppercase(str));
            } else {
                return Conversions::pack_string_simple(Transliterator::lowercase(str));
            }
        } else {
            return ObjectId::get_null();
        }
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    void print(std::ostream& os, std::vector<BindingIter*> ops) const override
    {
        if (upper) {
            os << "UPPER";
        } else {
            os << "LOWER";
        }
        os << "(";
        expr->print(os, ops);
        os << ")";
    }
};
} // namespace GQL
