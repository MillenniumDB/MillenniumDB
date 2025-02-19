#pragma once

#include <cassert>
#include <memory>
#include <string>

#include "misc/transliterator.h"
#include "graph_models/gql/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace GQL {
class BindingExprFold : public BindingExpr {
private:
    static std::string lcase(const std::string& str) {
        return Transliterator::get_instance()->lowercase(str);
    }

    static std::string ucase(const std::string& str) {
        return Transliterator::get_instance()->uppercase(str);
    }

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
                return Conversions::pack_string_simple(ucase(str));
            } else {
                return Conversions::pack_string_simple(lcase(str));
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