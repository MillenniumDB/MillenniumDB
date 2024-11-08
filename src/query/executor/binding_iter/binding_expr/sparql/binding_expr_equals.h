#pragma once

#include <algorithm>
#include <cctype>
#include <cmath>
#include <memory>
#include <set>

#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace SPARQL {

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

    BindingExprEquals(
        std::unique_ptr<BindingExpr> lhs,
        std::unique_ptr<BindingExpr> rhs
    ) :
        lhs(std::move(lhs)),
        rhs(std::move(rhs)) { }

    bool datatype_has_special_representation(const std::string& datatype) {
        if (known_datatypes.find(datatype) != known_datatypes.end()) {
            return true;
        }
        return false;
    }

    ObjectId eval(const Binding& binding) override {
        auto lhs_oid = lhs->eval(binding);
        auto rhs_oid = rhs->eval(binding);

        auto lhs_subtype = RDF_OID::get_generic_sub_type(lhs_oid);
        auto rhs_subtype = RDF_OID::get_generic_sub_type(rhs_oid);

        auto lhs_generic_type = RDF_OID::get_generic_type(lhs_oid);
        auto rhs_generic_type = RDF_OID::get_generic_type(rhs_oid);

        // Nulls are not equal to anything, including other nulls.
        if (lhs_oid.is_null() || rhs_oid.is_null()) {
            return ObjectId::get_null();
        }

        // Check if the ids are equal
        if (lhs_oid == rhs_oid) {
            // For floats NaN != NaN, so we have to check for that case.
            if (lhs_subtype == RDF_OID::GenericSubType::FLOAT) {
                auto f = Conversions::unpack_float(lhs_oid);
                if (std::isnan(f)) {
                    return SPARQL::Conversions::pack_bool(false);
                }
            }
            else if (lhs_subtype == RDF_OID::GenericSubType::DOUBLE) {
                auto d = Conversions::unpack_double(lhs_oid);
                if (std::isnan(d)) {
                    return SPARQL::Conversions::pack_bool(false);
                }
            }
            return SPARQL::Conversions::pack_bool(true);
        }

        // If both types are numeric we need to do a numeric comparison
        if (lhs_generic_type == RDF_OID::GenericType::NUMERIC &&
            rhs_generic_type == RDF_OID::GenericType::NUMERIC)
        {
            auto optype = Conversions::calculate_optype(lhs_oid, rhs_oid);
            switch (optype) {
            case Conversions::OPTYPE_INTEGER: {
                auto lhs = Conversions::to_integer(lhs_oid);
                auto rhs = Conversions::to_integer(rhs_oid);
                return SPARQL::Conversions::pack_bool(lhs == rhs);
            }
            case Conversions::OPTYPE_DECIMAL: {
                auto lhs = Conversions::to_decimal(lhs_oid);
                auto rhs = Conversions::to_decimal(rhs_oid);
                return SPARQL::Conversions::pack_bool(lhs == rhs);
            }
            case Conversions::OPTYPE_FLOAT: {
                auto lhs = Conversions::to_float(lhs_oid);
                auto rhs = Conversions::to_float(rhs_oid);
                return SPARQL::Conversions::pack_bool(lhs == rhs);
            }
            case Conversions::OPTYPE_DOUBLE: {
                auto lhs = Conversions::to_double(lhs_oid);
                auto rhs = Conversions::to_double(rhs_oid);
                return SPARQL::Conversions::pack_bool(lhs == rhs);
            }
            case Conversions::OPTYPE_INVALID: {
                return ObjectId::get_null();
            }
            default:
                throw std::runtime_error("This should never happen.");
            }
        }

        // Handle date, time, dateTime and dateTimeStamp
        if (lhs_generic_type == RDF_OID::GenericType::DATE &&
            rhs_generic_type == RDF_OID::GenericType::DATE)
        {
            bool error;
            auto res = DateTime(lhs_oid.id).compare<DateTimeComparisonMode::StrictEquality>(DateTime(rhs_oid.id), &error) == 0;
            if (error) {
                return ObjectId::get_null();
            }
            return SPARQL::Conversions::pack_bool(res);
        }

        // Next we handle objects of the same type
        if (lhs_subtype == rhs_subtype) {
            // Add this point we have objects of the same type,
            // but with different values, otherwise we would
            // already have returned earlier.

            // We have to datatype with unknown semantics specially
            if (lhs_subtype == RDF_OID::GenericSubType::STRING_DATATYPE) {
                auto&& [lhs_datatype, lhs_str] = Conversions::unpack_string_datatype(lhs_oid);
                auto&& [rhs_datatype, rhs_str] = Conversions::unpack_string_datatype(rhs_oid);
                // Check for ill-typed literals
                if (datatype_has_special_representation(lhs_datatype) ||
                    datatype_has_special_representation(rhs_datatype))
                {
                    return ObjectId::get_null();
                }

                if (lhs_datatype == rhs_datatype && lhs_str == rhs_str) {
                    return SPARQL::Conversions::pack_bool(true);
                } else {
                    // We can not conclude anything because the datatypes have unknown semantics.
                    return ObjectId::get_null();
                }
            }

            // We have to handle possible case difference of language tags
            if (lhs_subtype == RDF_OID::GenericSubType::STRING_LANG) {
                auto&& [lhs_lang, lhs_str] = Conversions::unpack_string_lang(lhs_oid);
                auto&& [rhs_lang, rhs_str] = Conversions::unpack_string_lang(rhs_oid);

                std::transform(lhs_lang.begin(), lhs_lang.end(), lhs_lang.begin(), ::tolower);
                std::transform(rhs_lang.begin(), rhs_lang.end(), rhs_lang.begin(), ::tolower);

                bool lang_equal = lhs_lang == rhs_lang;
                bool str_equal  = lhs_str  == rhs_str;
                return SPARQL::Conversions::pack_bool(lang_equal && str_equal);
            }

            // We handled all special cases for objects of the same type,
            // we can conclude that the two objects are different.
            return SPARQL::Conversions::pack_bool(false);

        }

        // We have to handle simple literals and xsd:string specially
        if ((lhs_subtype == RDF_OID::GenericSubType::STRING_XSD ||
             lhs_subtype == RDF_OID::GenericSubType::STRING_SIMPLE)
            &&
            (rhs_subtype == RDF_OID::GenericSubType::STRING_XSD ||
             rhs_subtype == RDF_OID::GenericSubType::STRING_SIMPLE))
        {
            auto equals = Conversions::to_lexical_str(lhs_oid) == Conversions::to_lexical_str(rhs_oid);
            return SPARQL::Conversions::pack_bool(equals);
        }

        if (lhs_subtype == RDF_OID::GenericSubType::BLANK ||
            lhs_subtype == RDF_OID::GenericSubType::IRI ||
            lhs_subtype == RDF_OID::GenericSubType::STRING_LANG)
        {
            return SPARQL::Conversions::pack_bool(false);
        }
        if (rhs_subtype == RDF_OID::GenericSubType::BLANK ||
            rhs_subtype == RDF_OID::GenericSubType::IRI ||
            rhs_subtype == RDF_OID::GenericSubType::STRING_LANG)
        {
            return SPARQL::Conversions::pack_bool(false);
        }

        // At this point only string literals with different datatypes are left.
        return ObjectId::get_null();
    }

    void accept_visitor(BindingExprVisitor& visitor) override {
        visitor.visit(*this);
    }
};
} // namespace SPARQL
