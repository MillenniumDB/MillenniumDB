#pragma once

#include <algorithm>
#include <cctype>
#include <cmath>
#include <memory>
#include <set>

#include "graph_models/gql/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace GQL {

class BindingExprNullIf : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> lhs;
    std::unique_ptr<BindingExpr> rhs;

    BindingExprNullIf(std::unique_ptr<BindingExpr> lhs, std::unique_ptr<BindingExpr> rhs) :
        lhs(std::move(lhs)),
        rhs(std::move(rhs))
    { }

    ObjectId eval(const Binding& binding) override
    {
        auto lhs_oid = lhs->eval(binding);
        auto rhs_oid = rhs->eval(binding);

        auto lhs_subtype = GQL_OID::get_generic_sub_type(lhs_oid);
        auto rhs_subtype = GQL_OID::get_generic_sub_type(rhs_oid);

        auto lhs_generic_type = GQL_OID::get_generic_type(lhs_oid);
        auto rhs_generic_type = GQL_OID::get_generic_type(rhs_oid);

        if ((lhs_oid.is_null() && rhs_oid.is_null()) || (lhs_oid.is_null() && !rhs_oid.is_null())) {
            return ObjectId::get_null();
        }

        if ((!lhs_oid.is_null() && rhs_oid.is_null())) {
            if (lhs_generic_type == GQL_OID::GenericType::NUMERIC) {
                switch (lhs_subtype) {
                case GQL_OID::GenericSubType::INTEGER: {
                    return GQL::Conversions::pack_int(GQL::Conversions::to_integer(lhs_oid));
                }
                case GQL_OID::GenericSubType::FLOAT: {
                    return GQL::Conversions::pack_float(GQL::Conversions::to_float(lhs_oid));
                }
                case GQL_OID::GenericSubType::DECIMAL: {
                    return GQL::Conversions::pack_decimal(GQL::Conversions::to_decimal(lhs_oid));
                }
                case GQL_OID::GenericSubType::DOUBLE: {
                    return GQL::Conversions::pack_double(GQL::Conversions::to_double(lhs_oid));
                }
                default:
                    throw std::runtime_error("This should never happen.");
                }
            } else if (lhs_generic_type == GQL_OID::GenericType::BOOL) {
                auto lhs = GQL::Conversions::to_boolean(lhs_oid);
                if (lhs == GQL::Conversions::pack_bool(true)) {
                    return GQL::Conversions::pack_bool(true);
                } else if (lhs == GQL::Conversions::pack_bool(false)) {
                    return GQL::Conversions::pack_bool(false);
                }
            } else if (lhs_generic_type == GQL_OID::GenericType::DATE) {
                return GQL::Conversions::pack_date(DateTime(lhs_oid.id));
            } else if (lhs_generic_type == GQL_OID::GenericType::STRING) {
                return GQL::Conversions::pack_string_simple(GQL::Conversions::unpack_string(lhs_oid));
            }
        }

        if (lhs_generic_type == GQL_OID::GenericType::BOOL
            && rhs_generic_type == GQL_OID::GenericType::BOOL)
        {
            auto lhs = GQL::Conversions::to_boolean(lhs_oid);
            auto rhs = GQL::Conversions::to_boolean(rhs_oid);
            if (lhs == rhs) {
                return ObjectId::get_null();
            }
            if (lhs == GQL::Conversions::pack_bool(true)) {
                return GQL::Conversions::pack_bool(true);
            } else if (lhs == GQL::Conversions::pack_bool(false)) {
                return GQL::Conversions::pack_bool(false);
            }
        }

        if (lhs_generic_type == GQL_OID::GenericType::NUMERIC
            && rhs_generic_type == GQL_OID::GenericType::NUMERIC)
        {
            auto optype = GQL::Conversions::calculate_optype(lhs_oid, rhs_oid);
            switch (optype) {
            case GQL::Conversions::OpType::INTEGER: {
                auto lhs = GQL::Conversions::to_integer(lhs_oid);
                auto rhs = GQL::Conversions::to_integer(rhs_oid);
                if (lhs == rhs) {
                    return ObjectId::get_null();
                }
                return GQL::Conversions::pack_int(lhs);
            }
            case GQL::Conversions::OpType::DECIMAL: {
                auto lhs = GQL::Conversions::to_decimal(lhs_oid);
                auto rhs = GQL::Conversions::to_decimal(rhs_oid);
                if (lhs == rhs) {
                    return ObjectId::get_null();
                }
                return GQL::Conversions::pack_decimal(lhs);
            }
            case GQL::Conversions::OpType::FLOAT: {
                auto lhs = GQL::Conversions::to_float(lhs_oid);
                auto rhs = GQL::Conversions::to_float(rhs_oid);
                if (lhs == rhs) {
                    return ObjectId::get_null();
                }
                return GQL::Conversions::pack_float(lhs);
            }
            case GQL::Conversions::OpType::DOUBLE: {
                auto lhs = GQL::Conversions::to_double(lhs_oid);
                auto rhs = GQL::Conversions::to_double(rhs_oid);
                if (lhs == rhs) {
                    return ObjectId::get_null();
                }
                return GQL::Conversions::pack_double(lhs);
            }
            case GQL::Conversions::OpType::INVALID: {
                return ObjectId::get_null();
            }
            default:
                throw std::runtime_error("This should never happen.");
            }
        }

        // Handle date, time, dateTime and dateTimeStamp
        if (lhs_generic_type == GQL_OID::GenericType::DATE && rhs_generic_type == GQL_OID::GenericType::DATE)
        {
            bool error;
            auto res = DateTime(lhs_oid.id)
                           .compare<DateTimeComparisonMode::StrictEquality>(DateTime(rhs_oid.id), &error)
                    == 0;
            if (error) {
                return ObjectId::get_null();
            }
            if (res) {
                return ObjectId::get_null();
            }
            return GQL::Conversions::pack_date(DateTime(lhs_oid.id));
        }

        // We have to handle simple literals
        if (lhs_subtype == GQL_OID::GenericSubType::STRING_SIMPLE
            && rhs_subtype == GQL_OID::GenericSubType::STRING_SIMPLE)
        {
            auto equals = GQL::Conversions::to_lexical_str(lhs_oid)
                       == GQL::Conversions::to_lexical_str(rhs_oid);
            if (equals) {
                return ObjectId::get_null();
            }
            return GQL::Conversions::pack_string_simple(GQL::Conversions::unpack_string(lhs_oid));
        }

        // At this point only string literals with different datatypes are left.
        return GQL::Conversions::pack_string_simple(GQL::Conversions::unpack_string(lhs_oid));
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }
};
} // namespace GQL
