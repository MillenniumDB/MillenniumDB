#pragma once

#include <cctype>
#include <memory>

#include "graph_models/object_id.h"
#include "graph_models/rdf_model/conversions.h"
#include "graph_models/rdf_model/datatypes/datetime.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"
#include "storage/unified_storage.h"

namespace SPARQL {

// The following datatypes have know semantics and special handling in MillenniumDB.
// If literals with any of the following datatypes have an invalid format, then during
// parsing they are stored as a string with datatype instead of the specialized format.
// This means any strings with datatype with any of the following datatypes are ill-typed
// literals. They still have to be stored, but equality testing on them returns an error.
static const std::string know_datatype_names[] = {
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
private:
    std::set<uint16_t> known_datatypes;

public:
    std::unique_ptr<BindingExpr> lhs;
    std::unique_ptr<BindingExpr> rhs;

    BindingExprEquals(std::unique_ptr<BindingExpr> lhs, std::unique_ptr<BindingExpr> rhs) :
        lhs(std::move(lhs)), rhs(std::move(rhs)) {
            for (auto& name : know_datatype_names) {
                known_datatypes.insert(UnifiedStorage::get_datatype_id(name));
            }
        }

    bool datatype_has_special_representation(uint16_t datatype) {
        if (known_datatypes.find(datatype) != known_datatypes.end()) {
            return true;
        }
        return false;
    }

    ObjectId eval(const Binding& binding) override {
        auto lhs_oid = lhs->eval(binding);
        auto rhs_oid = rhs->eval(binding);

        auto lhs_subtype = lhs_oid.get_sub_type();
        auto rhs_subtype = rhs_oid.get_sub_type();

        auto lhs_generic_type = lhs_oid.get_generic_type();
        auto rhs_generic_type = rhs_oid.get_generic_type();

        // Nulls are not equal to anything, including other nulls.
        if (lhs_oid.is_null() || rhs_oid.is_null()) {
            return ObjectId::get_null();
        }

        // Check if the ids are equal
        if (lhs_oid == rhs_oid) {
            // For floats NaN != NaN, so we have to check for that case.
            if (lhs_subtype == ObjectId::MASK_FLOAT) {
                auto f = Conversions::unpack_float(lhs_oid);
                if (isnan(f)) {
                    return ObjectId(ObjectId::BOOL_FALSE);
                }
            }
            else if (lhs_subtype == ObjectId::MASK_DOUBLE) {
                auto d = Conversions::unpack_double(lhs_oid);
                if (isnan(d)) {
                    return ObjectId(ObjectId::BOOL_FALSE);
                }
            }
            return ObjectId(ObjectId::BOOL_TRUE);
        }

        // If both types are numeric we need to do a numeric comparison
        if (lhs_oid.is_numeric() && rhs_oid.is_numeric()) {
            auto optype = Conversions::calculate_optype(lhs_oid, rhs_oid);
            switch (optype) {
            case Conversions::OPTYPE_INTEGER: {
                auto lhs = Conversions::to_integer(lhs_oid);
                auto rhs = Conversions::to_integer(rhs_oid);
                return ObjectId(ObjectId::MASK_BOOL | (lhs == rhs));
            }
            case Conversions::OPTYPE_DECIMAL: {
                auto lhs = Conversions::to_decimal(lhs_oid);
                auto rhs = Conversions::to_decimal(rhs_oid);
                return ObjectId(ObjectId::MASK_BOOL | (lhs == rhs));
            }
            case Conversions::OPTYPE_FLOAT: {
                auto lhs = Conversions::to_float(lhs_oid);
                auto rhs = Conversions::to_float(rhs_oid);
                return ObjectId(ObjectId::MASK_BOOL | (lhs == rhs));
            }
            case Conversions::OPTYPE_DOUBLE: {
                auto lhs = Conversions::to_double(lhs_oid);
                auto rhs = Conversions::to_double(rhs_oid);
                return ObjectId(ObjectId::MASK_BOOL | (lhs == rhs));
            }
            case Conversions::OPTYPE_INVALID: {
                return ObjectId::get_null();
            }
            default:
                throw std::runtime_error("This should never happen.");
            }
        }

        // Handle date, time, dateTime and dateTimeStamp
        if (lhs_generic_type == ObjectId::MASK_DT && rhs_generic_type == ObjectId::MASK_DT) {
            bool error;
            auto res = DateTime(lhs_oid.id).compare<DateTimeComparisonMode::StrictEquality>(DateTime(rhs_oid.id), &error) == 0;
            if (error) {
                return ObjectId::get_null();
            }
            return ObjectId(ObjectId::MASK_BOOL | res);
        }

        // Next we handle objects of the same type
        if (lhs_subtype == rhs_subtype) {
            // Add this point we have objects of the same type,
            // but with different values, otherwise we would
            // already have returned earlier.

            // We have to datatype with unknown semantics specially
            if (lhs_subtype == ObjectId::MASK_STRING_DATATYPE) {
                auto [lhs_datatype, lhs_str] = Conversions::unpack_string_datatype(lhs_oid);
                auto [rhs_datatype, rhs_str] = Conversions::unpack_string_datatype(rhs_oid);
                // Check for ill-typed literals
                if (datatype_has_special_representation(lhs_datatype) || datatype_has_special_representation(rhs_datatype)) {
                    return ObjectId::get_null();
                }

                if (lhs_datatype == rhs_datatype && lhs_str == rhs_str) {
                    return ObjectId(ObjectId::BOOL_TRUE);
                } else {
                    // We can not conclude anything because the datatypes have unknown semantics.
                    return ObjectId::get_null();
                }
            }

            // We have to handle possible case difference of language tags
            if (lhs_subtype == ObjectId::MASK_STRING_LANG) {
                auto [lhs_lang, lhs_str] = Conversions::unpack_string_lang(lhs_oid);
                auto [rhs_lang, rhs_str] = Conversions::unpack_string_lang(rhs_oid);

                auto lhs_lang_iter = UnifiedStorage::get_language_char_iter(lhs_lang);
                auto rhs_lang_iter = UnifiedStorage::get_language_char_iter(rhs_lang);

                bool lang_equal = true;
                while (true) {
                    auto lhs_char = lhs_lang_iter->next_char();
                    auto rhs_char = rhs_lang_iter->next_char();

                    if (lhs_char == rhs_char) {
                        if (lhs_char == '\0') {
                            // Reached the end of both strings
                            break;
                        } else {
                            // We have to keep checking
                            continue;
                        }
                    }

                    // The two chars are different.
                    // We have to check if they only differ in case.
                    lhs_char = tolower(lhs_char);
                    rhs_char = tolower(rhs_char);

                    if (lhs_char == rhs_char) {
                        // They only differ in case
                        continue;
                    } else {
                        // They are different
                        lang_equal = false;
                        break;
                    }
                }

                auto str_equal = lhs_str == rhs_str;
                return ObjectId(ObjectId::MASK_BOOL | (lang_equal && str_equal));
            }

            // We handled all special cases for objects of the same type,
            // we can conclude that the two objects are different.
            return ObjectId(ObjectId::BOOL_FALSE);

        }

        // We have to handle simple literals and xsd:string specially
        if ((lhs_subtype == ObjectId::MASK_STRING_XSD || lhs_subtype == ObjectId::MASK_STRING_SIMPLE) &&
            (rhs_subtype == ObjectId::MASK_STRING_XSD || rhs_subtype == ObjectId::MASK_STRING_SIMPLE)) {

            auto equals = Conversions::to_lexical_str(lhs_oid) == Conversions::to_lexical_str(rhs_oid);
            return ObjectId(ObjectId::MASK_BOOL | equals);
        }

        if (lhs_subtype == ObjectId::MASK_ANON || lhs_subtype == ObjectId::MASK_IRI || lhs_subtype == ObjectId::MASK_STRING_LANG) {
            return ObjectId(ObjectId::BOOL_FALSE);
        }
        if (rhs_subtype == ObjectId::MASK_ANON || rhs_subtype == ObjectId::MASK_IRI || rhs_subtype == ObjectId::MASK_STRING_LANG) {
            return ObjectId(ObjectId::BOOL_FALSE);
        }

        // At this point only string literals with different datatypes are left.
        return ObjectId::get_null();
    }

    std::ostream& print_to_ostream(std::ostream& os) const override {
        os << '(' << *lhs << '=' << *rhs << ')';
        return os;
    }
};
} // namespace SPARQL
