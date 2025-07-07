#pragma once

#include <cassert>
#include <memory>
#include <unicode/unistr.h>

#include "graph_models/gql/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace GQL {
class BindingExprLength : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    BindingExprLength(std::unique_ptr<BindingExpr> expr) :
        expr(std::move(expr))
    { }

    ObjectId eval(const Binding& binding) override
    {
        auto expr_oid = expr->eval(binding);
        auto expr_generic_type = GQL_OID::get_generic_type(expr_oid);

        if (expr_generic_type == GQL_OID::GenericType::STRING) {
            auto str = GQL::Conversions::unpack_string(expr_oid);
            icu::UnicodeString unicode_str = icu::UnicodeString::fromUTF8(str);
            return GQL::Conversions::pack_int(unicode_str.length());
        } else {
            return ObjectId::get_null();
        }
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }
};
} // namespace GQL
