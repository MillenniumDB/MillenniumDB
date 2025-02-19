#pragma once

#include <vector>
#include <memory>

#include "graph_models/common/datatypes/datetime.h"
#include "graph_models/gql/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace GQL {
class BindingExprCoalesce : public BindingExpr {
public:
    std::vector<std::unique_ptr<BindingExpr>> expressions;

    BindingExprCoalesce(std::vector<std::unique_ptr<BindingExpr>> expressions) :
        expressions(std::move(expressions))
    { }

    ObjectId eval(const Binding& binding) override
    {
        for (const auto& expr : expressions) {
            auto value = expr->eval(binding);
            if (!value.is_null()) {
                auto generic_type = GQL_OID::get_generic_type(value);

                switch (generic_type) {
                case GQL_OID::GenericType::STRING: {
                    return GQL::Conversions::pack_string_simple(
                        GQL::Conversions::unpack_string(value));
                }
                case GQL_OID::GenericType::NUMERIC: {
                    auto numeric_type = GQL_OID::get_generic_sub_type(value);
                    if (numeric_type == GQL_OID::GenericSubType::INTEGER) {
                        return GQL::Conversions::pack_int(
                            GQL::Conversions::to_integer(value));
                    } else if (numeric_type == GQL_OID::GenericSubType::FLOAT) {
                        return GQL::Conversions::pack_float(
                            GQL::Conversions::to_float(value));
                    } else if (numeric_type == GQL_OID::GenericSubType::DOUBLE) {
                        return GQL::Conversions::pack_double(
                            GQL::Conversions::to_double(value));
                    }
                    break;
                }
                case GQL_OID::GenericType::BOOL: {
                    auto expr_bool = GQL::Conversions::to_boolean(value);
                    if (expr_bool == GQL::Conversions::pack_bool(true)) {
                        return GQL::Conversions::pack_bool(true);
                    } else if (expr_bool == GQL::Conversions::pack_bool(false)) {
                        return GQL::Conversions::pack_bool(false);
                    }
                    break;
                }
                case GQL_OID::GenericType::DATE: {
                    return GQL::Conversions::pack_date(DateTime(value));
                }
                default:
                    return ObjectId::get_null();
                }
            }
        }
        return ObjectId::get_null();
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }
};
} // namespace GQL
