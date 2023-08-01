#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>

#include "graph_models/object_id.h"
#include "graph_models/rdf_model/conversions.h"
#include "graph_models/rdf_model/datatypes/datetime.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"
#include "query/parser/expr/sparql/builtin_call/expr_cast.h"

// https://www.w3.org/TR/sparql11-query/#FunctionMapping

namespace SPARQL {
class BindingExprCast : public BindingExpr {
public:
    CastType cast_type;
    std::unique_ptr<BindingExpr> expr;

    BindingExprCast(CastType cast_type, std::unique_ptr<BindingExpr> expr) :
        cast_type(cast_type),
        expr     (std::move(expr)) { }

    ObjectId eval(const Binding& binding) override {
        auto oid = expr->eval(binding);
        if (oid.is_null()) {
            return oid;
        }

        auto subtype = oid.get_sub_type();
        auto value = oid.get_value();

        switch (subtype) {
        case ObjectId::MASK_STRING_XSD:
        case ObjectId::MASK_STRING_SIMPLE: {
            switch (cast_type) {
            case CastType::xsd_string: {
                if (subtype == ObjectId::MASK_STRING_XSD) {
                    return oid;
                }
                return ObjectId((oid.id & (~ObjectId::SUB_TYPE_MASK)) | ObjectId::MASK_STRING_XSD);
            }
            case CastType::xsd_float: {
                auto str = Conversions::to_lexical_str(oid);
                float flt;
                size_t idx;
                try { flt = std::stof(str, &idx); }
                catch (...) { return ObjectId::get_null(); }
                if (idx != str.size()) {
                    return ObjectId::get_null();
                }
                return Conversions::pack_float(flt);
            }
            case CastType::xsd_double: {
                auto str = Conversions::to_lexical_str(oid);
                double dbl;
                size_t idx;
                try { dbl = std::stod(str, &idx); }
                catch (...) { return ObjectId::get_null(); }
                if (idx != str.size()) {
                    return ObjectId::get_null();
                }
                return Conversions::pack_double(dbl);
            }
            case CastType::xsd_decimal: {
                auto str = Conversions::to_lexical_str(oid);
                bool error;
                Decimal dec(str, &error);
                if (error) {
                    return ObjectId::get_null();
                }
                return Conversions::pack_decimal(dec);
            }
            case CastType::xsd_integer: {
                auto str = Conversions::to_lexical_str(oid);
                try {
                    size_t pos;
                    int64_t n = std::stoll(str, &pos);
                    // Check if the whole string was parsed
                    if (pos != str.size())
                        return ObjectId::get_null();
                    return Conversions::pack_int(n);
                } catch (std::out_of_range& e) {
                    return ObjectId::get_null();
                } catch (std::invalid_argument& e) {
                    return ObjectId::get_null();
                }
            }
            case CastType::xsd_dateTime: {
                auto str = Conversions::to_lexical_str(oid);
                auto dt = DateTime::from_dateTime(str);
                return ObjectId(dt);
            }
            case CastType::xsd_boolean: {
                auto str = Conversions::to_lexical_str(oid);
                if (str == "0" || str == "false") {
                    return ObjectId(ObjectId::BOOL_FALSE);
                } else if (str == "1" || str == "true") {
                    return ObjectId(ObjectId::BOOL_TRUE);
                } else {
                    return ObjectId::get_null();
                }
            }
            default: return ObjectId::get_null();
            }
        }
        case ObjectId::MASK_FLOAT: {
            switch (cast_type) {
            case CastType::xsd_string: return Conversions::pack_string_xsd(Conversions::to_lexical_str(oid));
            case CastType::xsd_float: return oid;
            case CastType::xsd_double: {
                float flt = Conversions::unpack_float(oid);
                return Conversions::pack_double(flt);
            }
            case CastType::xsd_decimal:  {
                auto str = Conversions::to_lexical_str(oid);
                bool error;
                Decimal dec(str, &error);
                if (error) {
                    return ObjectId::get_null();
                }
                return Conversions::pack_decimal(dec);
            }
            case CastType::xsd_integer: {
                auto flt = Conversions::unpack_float(oid);
                if (std::abs(flt) > static_cast<float>(Conversions::INTEGER_MAX)) {
                    return ObjectId::get_null();
                }
                return Conversions::pack_int(static_cast<int64_t>(flt));
            };
            case CastType::xsd_dateTime: return ObjectId::get_null();
            case CastType::xsd_boolean: return Conversions::to_boolean(oid);
            default: return ObjectId::get_null();
            }
        }
        case ObjectId::MASK_DOUBLE: {
            switch (cast_type) {
            case CastType::xsd_string: return Conversions::pack_string_xsd(Conversions::to_lexical_str(oid));
            case CastType::xsd_float:  {
                float flt = Conversions::unpack_double(oid);
                return Conversions::pack_float(flt);
            }
            case CastType::xsd_double: return oid;
            case CastType::xsd_decimal:  {
                auto str = Conversions::to_lexical_str(oid);
                bool error;
                Decimal dec(str, &error);
                if (error) {
                    return ObjectId::get_null();
                }
                return Conversions::pack_decimal(dec);
            }
            case CastType::xsd_integer: {
                auto dbl = Conversions::unpack_double(oid);
                if (std::abs(dbl) > static_cast<double>(Conversions::INTEGER_MAX)) {
                    return ObjectId::get_null();
                }
                return Conversions::pack_int(static_cast<int64_t>(dbl));
            };
            case CastType::xsd_dateTime: return ObjectId::get_null();
            case CastType::xsd_boolean: return Conversions::to_boolean(oid);
            default: return ObjectId::get_null();
            }
        }
        case ObjectId::MASK_DECIMAL: {
            switch (cast_type) {
            case CastType::xsd_string: return Conversions::pack_string_xsd(Conversions::to_lexical_str(oid));
            case CastType::xsd_float: {
                float flt = Conversions::unpack_decimal(oid).to_float();
                return Conversions::pack_float(flt);
            }
            case CastType::xsd_double: {
                double dbl = Conversions::unpack_decimal(oid).to_double();
                return Conversions::pack_double(dbl);
            }
            case CastType::xsd_decimal: return oid;
            case CastType::xsd_integer: {
                auto dec = Conversions::unpack_decimal(oid);
                bool error;
                int64_t i = dec.to_int(&error);
                if (error || std::abs(i) > Conversions::INTEGER_MAX) {
                    return ObjectId::get_null();
                }
                return Conversions::pack_int(i);
            }
            case CastType::xsd_dateTime: return ObjectId::get_null();
            case CastType::xsd_boolean: return Conversions::to_boolean(oid);
            default: return ObjectId::get_null();
            }
        }
        case ObjectId::MASK_INT: {
            switch (cast_type) {
            case CastType::xsd_string: return Conversions::pack_string_xsd(Conversions::to_lexical_str(oid));
            case CastType::xsd_float: {
                float flt = Conversions::unpack_int(oid);
                return Conversions::pack_float(flt);
            }
            case CastType::xsd_double: {
                double dbl = Conversions::unpack_int(oid);
                return Conversions::pack_double(dbl);
            }
            case CastType::xsd_decimal: {
                auto num = Conversions::unpack_int(oid);
                Decimal dec(num);
                return Conversions::pack_decimal(dec);
            }
            case CastType::xsd_integer: return oid;
            case CastType::xsd_dateTime: return ObjectId::get_null();
            case CastType::xsd_boolean: return Conversions::to_boolean(oid);
            default:
                return ObjectId::get_null();
            }
        }
        case ObjectId::MASK_DT_DATETIME: {
            switch (cast_type) {
            case CastType::xsd_string: return Conversions::pack_string_xsd(Conversions::to_lexical_str(oid));
            case CastType::xsd_float: return ObjectId::get_null();
            case CastType::xsd_double: return ObjectId::get_null();
            case CastType::xsd_decimal: return ObjectId::get_null();
            case CastType::xsd_integer: return ObjectId::get_null();
            case CastType::xsd_dateTime: return oid;
            case CastType::xsd_boolean: return ObjectId::get_null();
            default: return ObjectId::get_null();
            }
        }
        case ObjectId::MASK_BOOL: {
            switch (cast_type) {
            case CastType::xsd_string: return Conversions::pack_string_simple(Conversions::to_lexical_str(oid));
            case CastType::xsd_float: {
                float flt = oid.is_true() ? 1.0 : 0.0;
                return Conversions::pack_float(flt);
            }
            case CastType::xsd_double: {
               double dbl = oid.is_true() ? 1.0 : 0.0;
                return Conversions::pack_double(dbl);
            }
            case CastType::xsd_decimal: {
                int64_t num = oid.is_true() ? 1 : 0;
                Decimal dec(num);
                return Conversions::pack_decimal(dec);
            }
            case CastType::xsd_integer:{
                int64_t num = oid.is_true() ? 1 : 0;
                return Conversions::pack_int(num);
            }
            case CastType::xsd_dateTime: return ObjectId::get_null();
            case CastType::xsd_boolean: {
                return ObjectId(ObjectId::MASK_BOOL | value);
            }
            default: return ObjectId::get_null();
            }
        }
        case ObjectId::MASK_IRI: {
            switch (cast_type) {
            case CastType::xsd_string: return Conversions::pack_string_xsd(Conversions::to_lexical_str(oid));
            case CastType::xsd_float: return ObjectId::get_null();
            case CastType::xsd_double: return ObjectId::get_null();
            case CastType::xsd_decimal: return ObjectId::get_null();
            case CastType::xsd_integer: return ObjectId::get_null();
            case CastType::xsd_dateTime: return ObjectId::get_null();
            case CastType::xsd_boolean: return ObjectId::get_null();
            default: return ObjectId::get_null();
            }
        }
        default: return ObjectId::get_null();
        }
    }

    std::ostream& print_to_ostream(std::ostream& os) const override {
        return os << cast_type_to_string(cast_type) << '(' << *expr << ')';
    }
};
} // namespace SPARQL
