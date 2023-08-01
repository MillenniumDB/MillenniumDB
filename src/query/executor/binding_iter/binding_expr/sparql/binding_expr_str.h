#pragma once

#include <memory>

#include "graph_models/object_id.h"
#include "graph_models/rdf_model/conversions.h"
#include "graph_models/rdf_model/datatypes/datetime.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"
#include "storage/unified_storage.h"

namespace SPARQL {
class BindingExprStr : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    BindingExprStr(std::unique_ptr<BindingExpr> expr) :
        expr (std::move(expr)) { }

    ObjectId eval(const Binding& binding) override {
        auto expr_oid = expr->eval(binding);

        // TODO: Use Conversions::to_lexical_str?
        switch (expr_oid.get_sub_type()) {
        case ObjectId::MASK_IRI: {
            auto str = Conversions::unpack_iri(expr_oid);
            return Conversions::pack_string_simple(str);
        }
        case ObjectId::MASK_STRING_SIMPLE: {
            return expr_oid;
        }
        case ObjectId::MASK_STRING_XSD: {
            // Extract only the subtype bits from MASK_STRING_SIMPLE
            constexpr uint64_t sub_type = ObjectId::MASK_STRING_SIMPLE & 0x0C'00'00'00'00'00'00'00ULL;
            // Erase subtype bits
            expr_oid.id &= 0xF3'FF'FF'FF'FF'FF'FF'FFULL;
            // Apply MASK_STRING_SIMPLE subtype bits
            expr_oid.id |= sub_type;
            return expr_oid;
        }
        case ObjectId::MASK_STRING_LANG: {
            auto [_, str] = Conversions::unpack_string_lang(expr_oid);
            return Conversions::pack_string_simple(str);
        }
        case ObjectId::MASK_STRING_DATATYPE: {
            auto [_, str] = Conversions::unpack_string_datatype(expr_oid);
            return Conversions::pack_string_simple(str);
        }
        case ObjectId::MASK_INT: {
            auto i = Conversions::unpack_int(expr_oid);
            auto str = std::to_string(i);
            return Conversions::pack_string_simple(str);
        }
        case ObjectId::MASK_DECIMAL: {
            auto d = Conversions::unpack_decimal(expr_oid);
            auto str = d.to_string();
            return Conversions::pack_string_simple(str);
        }
        case ObjectId::MASK_FLOAT: {
            auto f = Conversions::unpack_float(expr_oid);
            std::stringstream ss;
            ss << f;
            auto str = ss.str();
            return Conversions::pack_string_simple(str);
        }
        case ObjectId::MASK_DOUBLE: {
            auto d = Conversions::unpack_double(expr_oid);
            std::stringstream ss;
            ss << d;
            auto str = ss.str();
            return Conversions::pack_string_simple(str);
        }
        case ObjectId::MASK_DT_DATE:
        case ObjectId::MASK_DT_DATETIME:
        case ObjectId::MASK_DT_TIME:
        case ObjectId::MASK_DT_DATETIMESTAMP: {
            auto str = DateTime(expr_oid).get_value_string();
            return Conversions::pack_string_simple(str);
        }
        case ObjectId::MASK_BOOL: {
            if (expr_oid.is_true()) {
                return Conversions::pack_string_simple("true");
            } else {
                return Conversions::pack_string_simple("false");
            }
        }
        default:
            return ObjectId::get_null();
        }
    }

    std::ostream& print_to_ostream(std::ostream& os) const override {
        os << "STR(" << *expr << ")";
        return os;
    }
};
} // namespace SPARQL
