#pragma once

#include <iomanip>
#include <memory>
#include <sstream>
#include <string>

#include "graph_models/object_id.h"
#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace SPARQL {
class BindingExprEncodeForUri : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    BindingExprEncodeForUri(std::unique_ptr<BindingExpr> expr) :
        expr (std::move(expr)) { }

    ObjectId eval(const Binding& binding) override {
        auto expr_oid = expr->eval(binding);

        std::string str;

        switch (expr_oid.get_sub_type()) {
        case ObjectId::MASK_STRING_SIMPLE: {
            str = Conversions::unpack_string_simple(expr_oid);
            break;
        }
        case ObjectId::MASK_STRING_XSD: {
            str = Conversions::unpack_string_xsd(expr_oid);
            break;
        }
        case ObjectId::MASK_STRING_LANG: {
            auto [l, s] = Conversions::unpack_string_lang(expr_oid);
            str = s;
            break;
        }
        default:
            return ObjectId::get_null();
        }

        std::ostringstream ss;
        ss << std::hex;
        ss << std::setfill('0');

        for (const char cc : str) {
            const unsigned char c = static_cast<unsigned char>(cc);
            if (c < 0x80 && (std::isalnum(c) || c == '-' || c == '.'  || c == '_'|| c == '~')) {
                ss << c;
                continue;
            }

            ss << std::uppercase;
            ss << '%' << std::setw(2) << int(c);
            ss << std::nouppercase;
        }

        return Conversions::pack_string_simple(ss.str());
    }

    std::ostream& print_to_ostream(std::ostream& os) const override {
        os << "ENCODE_FOR_URI(" << *expr << ")";
        return os;
    }
};
} // namespace SPARQL
