#pragma once

#include <memory>

#include "graph_models/gql/conversions.h"
#include "query/exceptions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace GQL {
class BindingExprSubStr : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;
    std::unique_ptr<BindingExpr> str_len;
    bool left;

    BindingExprSubStr(std::unique_ptr<BindingExpr> expr, std::unique_ptr<BindingExpr> str_len, bool left) :
        expr(std::move(expr)),
        str_len(std::move(str_len)),
        left(std::move(left))
    { }

    ObjectId eval(const Binding& binding) override
    {
        auto expr_oid = expr->eval(binding);
        auto expr_generic_type = GQL_OID::get_generic_type(expr_oid);

        auto len_oid = str_len->eval(binding);
        auto len_int = GQL::Conversions::to_integer(len_oid);

        if (len_int < 0) {
            throw QueryExecutionException("data exception — substring error (negative number)");
        }

        if (expr_generic_type == GQL_OID::GenericType::STRING) {
            auto str = GQL::Conversions::unpack_string(expr_oid);
            if (static_cast<std::size_t>(len_int) > str.size()) {
                return GQL::Conversions::pack_string_simple(str);
            }
            if (left == true) {
                return GQL::Conversions::pack_string_simple(str.substr(0, len_int));
            } else {
                return GQL::Conversions::pack_string_simple(str.substr(str.size() - len_int, len_int));
            }
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
