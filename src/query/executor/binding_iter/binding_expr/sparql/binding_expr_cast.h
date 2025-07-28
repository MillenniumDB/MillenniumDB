#pragma once

#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>

#include "graph_models/rdf_model/conversions.h"
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
        expr(std::move(expr))
    { }

    ObjectId eval(const Binding& binding) override
    {
        auto oid = expr->eval(binding);
        if (oid.is_null()) {
            return oid;
        }

        auto subtype = RDF_OID::get_generic_sub_type(oid);

        switch (subtype) {
        case RDF_OID::GenericSubType::STRING_XSD:
        case RDF_OID::GenericSubType::STRING_SIMPLE: {
            switch (cast_type) {
            case CastType::xsd_string: {
                if (subtype == RDF_OID::GenericSubType::STRING_XSD) {
                    return oid;
                }
                return Conversions::string_simple_to_xsd(oid);
            }
            case CastType::xsd_float: {
                auto str = Conversions::to_lexical_str(oid);
                float flt;
                size_t idx;
                try {
                    flt = std::stof(str, &idx);
                } catch (...) {
                    return ObjectId::get_null();
                }
                if (idx != str.size()) {
                    return ObjectId::get_null();
                }
                return Conversions::pack_float(flt);
            }
            case CastType::xsd_double: {
                auto str = Conversions::to_lexical_str(oid);
                double dbl;
                size_t idx;
                try {
                    dbl = std::stod(str, &idx);
                } catch (...) {
                    return ObjectId::get_null();
                }
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
                    return Conversions::pack_bool(false);
                } else if (str == "1" || str == "true") {
                    return Conversions::pack_bool(true);
                } else {
                    return ObjectId::get_null();
                }
            }
            case CastType::mdbtype_tensorFloat: {
                const auto str = Conversions::to_lexical_str(oid);
                bool error;
                const auto tensor = tensor::Tensor<float>::from_literal(str, &error);
                if (error) {
                    return ObjectId::get_null();
                }
                return Conversions::pack_tensor<float>(tensor);
            }
            case CastType::mdbtype_tensorDouble: {
                const auto str = Conversions::to_lexical_str(oid);
                bool error;
                const auto tensor = tensor::Tensor<double>::from_literal(str, &error);
                if (error) {
                    return ObjectId::get_null();
                }
                return Conversions::pack_tensor<double>(tensor);
            }
            default:
                return ObjectId::get_null();
            }
        }
        case RDF_OID::GenericSubType::FLOAT: {
            switch (cast_type) {
            case CastType::xsd_string:
                return Conversions::pack_string_xsd(Conversions::to_lexical_str(oid));
            case CastType::xsd_float:
                return oid;
            case CastType::xsd_double: {
                float flt = Conversions::unpack_float(oid);
                return Conversions::pack_double(flt);
            }
            case CastType::xsd_decimal: {
                float f = Conversions::unpack_float(oid);
                Decimal dec;
                switch (std::fpclassify(f)) {
                case FP_NORMAL: {
                    dec = Decimal(f);
                    break;
                }
                case FP_ZERO: {
                    // decimal is already 0
                    break;
                }
                case FP_INFINITE:
                case FP_NAN:
                case FP_SUBNORMAL: {
                    return ObjectId::get_null();
                }
                default:
                    return ObjectId::get_null();
                }
                return Conversions::pack_decimal(dec);
            }
            case CastType::xsd_integer: {
                auto flt = Conversions::unpack_float(oid);
                if (flt > static_cast<float>(INT64_MAX) || flt < static_cast<float>(INT64_MIN)) {
                    return ObjectId::get_null();
                }
                return Conversions::pack_int(static_cast<int64_t>(flt));
            }
            case CastType::xsd_boolean:
                return Conversions::to_boolean(oid);
            default:
                return ObjectId::get_null();
            }
        }
        case RDF_OID::GenericSubType::DOUBLE: {
            switch (cast_type) {
            case CastType::xsd_string:
                return Conversions::pack_string_xsd(Conversions::to_lexical_str(oid));
            case CastType::xsd_float: {
                float flt = static_cast<float>(Conversions::unpack_double(oid));
                return Conversions::pack_float(flt);
            }
            case CastType::xsd_double:
                return oid;
            case CastType::xsd_decimal: {
                double d = Conversions::unpack_double(oid);
                Decimal dec;
                switch (std::fpclassify(d)) {
                case FP_NORMAL: {
                    dec = Decimal(d);
                    break;
                }
                case FP_ZERO: {
                    // decimal is already 0
                    break;
                }
                case FP_INFINITE:
                case FP_NAN:
                case FP_SUBNORMAL: {
                    return ObjectId::get_null();
                }
                default:
                    return ObjectId::get_null();
                }
                return Conversions::pack_decimal(dec);
            }
            case CastType::xsd_integer: {
                auto dbl = Conversions::unpack_double(oid);
                if (dbl > static_cast<double>(INT64_MAX) || dbl < static_cast<double>(INT64_MIN)) {
                    return ObjectId::get_null();
                }
                return Conversions::pack_int(static_cast<int64_t>(dbl));
            }
            case CastType::xsd_boolean:
                return Conversions::to_boolean(oid);
            default:
                return ObjectId::get_null();
            }
        }
        case RDF_OID::GenericSubType::DECIMAL: {
            switch (cast_type) {
            case CastType::xsd_string:
                return Conversions::pack_string_xsd(Conversions::to_lexical_str(oid));
            case CastType::xsd_float: {
                float flt = Conversions::unpack_decimal(oid).to_float();
                return Conversions::pack_float(flt);
            }
            case CastType::xsd_double: {
                double dbl = Conversions::unpack_decimal(oid).to_double();
                return Conversions::pack_double(dbl);
            }
            case CastType::xsd_decimal:
                return oid;
            case CastType::xsd_integer: {
                auto dec = Conversions::unpack_decimal(oid);
                bool error;
                int64_t i = dec.to_int(&error);
                if (error) {
                    return ObjectId::get_null();
                }
                return Conversions::pack_int(i);
            }
            case CastType::xsd_boolean:
                return Conversions::to_boolean(oid);
            default:
                return ObjectId::get_null();
            }
        }
        case RDF_OID::GenericSubType::INTEGER: {
            switch (cast_type) {
            case CastType::xsd_string:
                return Conversions::pack_string_xsd(Conversions::to_lexical_str(oid));
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
            case CastType::xsd_integer:
                return oid;
            case CastType::xsd_boolean:
                return Conversions::to_boolean(oid);
            default:
                return ObjectId::get_null();
            }
        }
        case RDF_OID::GenericSubType::DATE: {
            switch (cast_type) {
            case CastType::xsd_string:
                return Conversions::pack_string_xsd(Conversions::to_lexical_str(oid));
            case CastType::xsd_dateTime:
                return oid;
            default:
                return ObjectId::get_null();
            }
        }
        case RDF_OID::GenericSubType::BOOL: {
            switch (cast_type) {
            case CastType::xsd_string:
                return Conversions::pack_string_simple(Conversions::to_lexical_str(oid));
            case CastType::xsd_float: {
                float flt = oid == Conversions::pack_bool(true) ? 1.0 : 0.0;
                return Conversions::pack_float(flt);
            }
            case CastType::xsd_double: {
                double dbl = oid == Conversions::pack_bool(true) ? 1.0 : 0.0;
                return Conversions::pack_double(dbl);
            }
            case CastType::xsd_decimal: {
                int64_t num = oid == Conversions::pack_bool(true) ? 1 : 0;
                Decimal dec(num);
                return Conversions::pack_decimal(dec);
            }
            case CastType::xsd_integer: {
                int64_t num = oid == Conversions::pack_bool(true) ? 1 : 0;
                return Conversions::pack_int(num);
            }
            case CastType::xsd_dateTime:
                return ObjectId::get_null();
            case CastType::xsd_boolean:
                return oid;
            default:
                return ObjectId::get_null();
            }
        }
        case RDF_OID::GenericSubType::IRI: {
            switch (cast_type) {
            case CastType::xsd_string:
                return Conversions::pack_string_xsd(Conversions::to_lexical_str(oid));
            default:
                return ObjectId::get_null();
            }
        }
        case RDF_OID::GenericSubType::TENSOR_FLOAT: {
            switch (cast_type) {
            case CastType::xsd_boolean:
                return Conversions::to_boolean(oid);
            case CastType::xsd_string:
                return Conversions::pack_string_xsd(Conversions::to_lexical_str(oid));
            case CastType::mdbtype_tensorFloat:
                return oid;
            case CastType::mdbtype_tensorDouble: {
                const auto casted = Conversions::to_tensor<double>(oid);
                return Conversions::pack_tensor<double>(casted);
            }
            default:
                return ObjectId::get_null();
            }
        }
        case RDF_OID::GenericSubType::TENSOR_DOUBLE: {
            switch (cast_type) {
            case CastType::xsd_boolean:
                return Conversions::to_boolean(oid);
            case CastType::xsd_string:
                return Conversions::pack_string_xsd(Conversions::to_lexical_str(oid));
            case CastType::mdbtype_tensorFloat: {
                const auto tensor = Conversions::to_tensor<float>(oid);
                return Conversions::pack_tensor<float>(tensor);
            }
            case CastType::mdbtype_tensorDouble:
                return oid;
            default:
                return ObjectId::get_null();
            }
        }
        default:
            return ObjectId::get_null();
        }
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    void print(std::ostream& os, std::vector<BindingIter*> ops) const override
    {
        os << cast_type_to_string(cast_type) << '(';
        expr->print(os, ops);
        os << ')';
    }
};
} // namespace SPARQL
