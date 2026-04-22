#pragma once

#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

#include <iomanip>
#include <memory>
#include <sstream>
#include <string>

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

        switch (expr_oid.subtype()) {
        case ObjectSubType::String:
        case ObjectSubType::StringXsd: {
            str = Conversions::unpack_string(expr_oid);
            break;
        }
        case ObjectSubType::StringLang: {
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

        return Conversions::pack_string(ss.str());
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    void print(std::ostream& os, std::vector<BindingIter*>& ops) const override
    {
        os << "ENCODE_FOR_URI(";
        expr->print(os, ops);
        os << ')';
    }
};
} // namespace SPARQL
