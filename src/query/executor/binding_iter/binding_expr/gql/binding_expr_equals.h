#pragma once

#include <algorithm>
#include <cctype>
#include <cmath>
#include <memory>
#include <set>

#include "graph_models/gql/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace GQL {

// The following datatypes have know semantics and special handling in MillenniumDB.
// If literals with any of the following datatypes have an invalid format, then during
// parsing they are stored as a string with datatype instead of the specialized format.
// This means any strings with datatype with any of the following datatypes are ill-typed
// literals. They still have to be stored, but equality testing on them returns an error.
static std::set<std::string> known_datatypes = {
    "http://www.w3.org/2001/XMLSchema#date",
    "http://www.w3.org/2001/XMLSchema#time",
    "http://www.w3.org/2001/XMLSchema#dateTime",
    "http://www.w3.org/2001/XMLSchema#dateTimeStamp",

    "http://www.w3.org/2001/XMLSchema#string",
    "http://www.w3.org/2001/XMLSchema#boolean",

    "http://www.w3.org/2001/XMLSchema#integer",
    "http://www.w3.org/2001/XMLSchema#decimal",
    "http://www.w3.org/2001/XMLSchema#float",
    "http://www.w3.org/2001/XMLSchema#double",

    "http://www.w3.org/2001/XMLSchema#byte",
    "http://www.w3.org/2001/XMLSchema#short",
    "http://www.w3.org/2001/XMLSchema#int",
    "http://www.w3.org/2001/XMLSchema#long",
    "http://www.w3.org/2001/XMLSchema#positiveInteger",
    "http://www.w3.org/2001/XMLSchema#negativeInteger",
    "http://www.w3.org/2001/XMLSchema#nonPositiveInteger",
    "http://www.w3.org/2001/XMLSchema#nonNegativeInteger",
    "http://www.w3.org/2001/XMLSchema#unsignedByte",
    "http://www.w3.org/2001/XMLSchema#unsignedShort",
    "http://www.w3.org/2001/XMLSchema#unsignedInt",
    "http://www.w3.org/2001/XMLSchema#unsignedLong",
};

class BindingExprEquals : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> lhs;
    std::unique_ptr<BindingExpr> rhs;

    BindingExprEquals(std::unique_ptr<BindingExpr> lhs, std::unique_ptr<BindingExpr> rhs) :
        lhs(std::move(lhs)),
        rhs(std::move(rhs))
    { }

    bool datatype_has_special_representation(const std::string& datatype)
    {
        if (known_datatypes.find(datatype) != known_datatypes.end()) {
            return true;
        }
        return false;
    }

    ObjectId eval(const Binding& binding) override
    {
        auto lhs_oid = lhs->eval(binding);
        auto rhs_oid = rhs->eval(binding);

        auto lhs_subtype = GQL_OID::get_generic_sub_type(lhs_oid);
        auto rhs_subtype = GQL_OID::get_generic_sub_type(rhs_oid);

        auto lhs_generic_type = GQL_OID::get_generic_type(lhs_oid);
        auto rhs_generic_type = GQL_OID::get_generic_type(rhs_oid);

        // Nulls are not equal to anything, including other nulls.
        if (lhs_oid.is_null() || rhs_oid.is_null()) {
            return ObjectId::get_null();
        }

        // Check if the ids are equal
        if (lhs_oid == rhs_oid) {
            // For floats NaN != NaN, so we have to check for that case.
            if (lhs_subtype == GQL_OID::GenericSubType::FLOAT) {
                auto f = GQL::Conversions::unpack_float(lhs_oid);
                if (std::isnan(f)) {
                    return GQL::Conversions::pack_bool(false);
                }
            } else if (lhs_subtype == GQL_OID::GenericSubType::DOUBLE) {
                auto d = GQL::Conversions::unpack_double(lhs_oid);
                if (std::isnan(d)) {
                    return GQL::Conversions::pack_bool(false);
                }
            }
            return GQL::Conversions::pack_bool(true);
        }

        if (lhs_generic_type == GQL_OID::GenericType::BOOL
            && rhs_generic_type == GQL_OID::GenericType::BOOL)
        {
            auto lhs = GQL::Conversions::to_boolean(lhs_oid);
            auto rhs = GQL::Conversions::to_boolean(rhs_oid);
            return GQL::Conversions::pack_bool(lhs == rhs);
        }

        // If both types are numeric we need to do a numeric comparison
        if (lhs_generic_type == GQL_OID::GenericType::NUMERIC
            && rhs_generic_type == GQL_OID::GenericType::NUMERIC)
        {
            auto optype = GQL::Conversions::calculate_optype(lhs_oid, rhs_oid);
            switch (optype) {
            case GQL::Conversions::OpType::INTEGER: {
                auto lhs = GQL::Conversions::to_integer(lhs_oid);
                auto rhs = GQL::Conversions::to_integer(rhs_oid);
                return GQL::Conversions::pack_bool(lhs == rhs);
            }
            case GQL::Conversions::OpType::DECIMAL: {
                auto lhs = GQL::Conversions::to_decimal(lhs_oid);
                auto rhs = GQL::Conversions::to_decimal(rhs_oid);
                return GQL::Conversions::pack_bool(lhs == rhs);
            }
            case GQL::Conversions::OpType::FLOAT: {
                auto lhs = GQL::Conversions::to_float(lhs_oid);
                auto rhs = GQL::Conversions::to_float(rhs_oid);
                return GQL::Conversions::pack_bool(lhs == rhs);
            }
            case GQL::Conversions::OpType::DOUBLE: {
                auto lhs = GQL::Conversions::to_double(lhs_oid);
                auto rhs = GQL::Conversions::to_double(rhs_oid);
                return GQL::Conversions::pack_bool(lhs == rhs);
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
            return GQL::Conversions::pack_bool(res);
        }

        // We have to handle simple literals
        if (lhs_subtype == GQL_OID::GenericSubType::STRING_SIMPLE
            && rhs_subtype == GQL_OID::GenericSubType::STRING_SIMPLE)
        {
            auto equals = GQL::Conversions::to_lexical_str(lhs_oid)
                       == GQL::Conversions::to_lexical_str(rhs_oid);
            return GQL::Conversions::pack_bool(equals);
        }

        // At this point only string literals with different datatypes are left.
        return ObjectId::get_null();
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }
};
} // namespace GQL
