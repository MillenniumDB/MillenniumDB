#pragma once

#include <iomanip>
#include <memory>
#include <sstream>
#include <string>

#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace SPARQL {
class BindingExprEncodeForUri : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    BindingExprEncodeForUri(std::unique_ptr<BindingExpr> expr) :
        expr(std::move(expr))
    { }

    ObjectId eval(const Binding& binding) override
    {
        auto expr_oid = expr->eval(binding);

        std::string str;

        switch (RDF_OID::get_generic_sub_type(expr_oid)) {
        case RDF_OID::GenericSubType::STRING_SIMPLE:
        case RDF_OID::GenericSubType::STRING_XSD: {
            str = Conversions::unpack_string(expr_oid);
            break;
        }
        case RDF_OID::GenericSubType::STRING_LANG: {
            auto&& [l, s] = Conversions::unpack_string_lang(expr_oid);
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
            if (c < 0x80 && (std::isalnum(c) || c == '-' || c == '.' || c == '_' || c == '~')) {
                ss << c;
                continue;
            }

            ss << std::uppercase;
            ss << '%' << std::setw(2) << int(c);
            ss << std::nouppercase;
        }

        return Conversions::pack_string_simple(ss.str());
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    void print(std::ostream& os, std::vector<BindingIter*> ops) const override
    {
        os << "ENCODE_FOR_URI(";
        expr->print(os, ops);
        os << ')';
    }
};
} // namespace SPARQL
