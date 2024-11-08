#pragma once

#include <cstddef>
#include <memory>
#include <regex>

#include "query/exceptions.h"
#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace SPARQL {
class BindingExprRegex : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr1;
    std::unique_ptr<BindingExpr> expr2;
    std::unique_ptr<BindingExpr> expr3;

    BindingExprRegex(
        std::unique_ptr<BindingExpr> expr1,
        std::unique_ptr<BindingExpr> expr2,
        std::unique_ptr<BindingExpr> expr3
    ) :
        expr1(std::move(expr1)),
        expr2(std::move(expr2)),
        expr3(std::move(expr3)) { }

    ObjectId eval(const Binding& binding) override {
        auto expr1_oid = expr1->eval(binding);
        auto expr2_oid = expr2->eval(binding);

        ObjectId expr3_oid;
        if (expr3) {
            expr3_oid = expr3->eval(binding);
        }

        std::string expr1_str;
        std::string expr2_str;
        std::string expr3_str;

        switch (RDF_OID::get_generic_sub_type(expr1_oid)) {
        case RDF_OID::GenericSubType::STRING_SIMPLE: {
        case RDF_OID::GenericSubType::STRING_XSD:
            expr1_str = Conversions::unpack_string(expr1_oid);
            break;
        }
        case RDF_OID::GenericSubType::STRING_LANG: {
            auto&& [lang, str] = Conversions::unpack_string_lang(expr1_oid);
            expr1_str = str;
            break;
        }
        default:
            return ObjectId::get_null();
        }

        if (RDF_OID::get_generic_sub_type(expr2_oid) != RDF_OID::GenericSubType::STRING_SIMPLE) {
            return ObjectId::get_null();
        }
        if (expr3 != nullptr &&
            RDF_OID::get_generic_sub_type(expr3_oid) != RDF_OID::GenericSubType::STRING_SIMPLE)
        {
            return ObjectId::get_null();
        }

        expr2_str = Conversions::unpack_string(expr2_oid);

        auto flags = std::regex::ECMAScript;
        if (expr3) {
            expr3_str = Conversions::unpack_string(expr3_oid);
            for (auto& f : expr3_str) {
                // TODO: implement all flags
                switch (f) {
                case 'i': { flags |= std::regex::icase; break; }
                // TODO: this should be in C++17 but does not compile
                // case 'm': { flags |= std::regex::multiline ; break; }
                default: {
                    std::stringstream ss;
                    ss << "Unsupported REGEX flag: \"" << f << '"';
                    throw QuerySemanticException(ss.str());
                }
                }
            }
        }

        // TODO: avoid expensive regex compilation.
        // - maybe add caching for regexes.
        // - if the regex is a simple string and not a variable recompiling for each eval is not necessary.
        std::regex reg(expr2_str, flags);

        // TODO: make sure regex behaves as in the standard.
        // An external regex library may be necessary.
        auto res = std::regex_search(expr1_str, reg);

        return Conversions::pack_bool(res);
    }

    void accept_visitor(BindingExprVisitor& visitor) override {
        visitor.visit(*this);
    }
};
} // namespace SPARQL
