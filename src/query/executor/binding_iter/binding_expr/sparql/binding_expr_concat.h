#pragma once

#include <memory>
#include <vector>

#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"


namespace SPARQL {

enum class ConcatStatus {
    UNSET,
    LANG,
    XSD,
    SIMPLE
};

// https://www.w3.org/TR/sparql11-query/#func-concat
// If all input literals are typed literals of type xsd:string, then the returned
// literal is also of type xsd:string, if all input literals are plain literals
// with identical language tag, then the returned literal is a plain literal
// with the same language tag, in all other cases, the returned literal is a
// simple literal.
class BindingExprConcat : public BindingExpr {
public:
    std::vector<std::unique_ptr<BindingExpr>> exprs;

    BindingExprConcat(std::vector<std::unique_ptr<BindingExpr>> exprs) :
        exprs(std::move(exprs)) { }

    ObjectId eval(const Binding& binding) override {
        std::string res;
        std::string res_lang;

        ConcatStatus status = ConcatStatus::UNSET;

        for (auto& expr : exprs) {
            auto expr_oid = expr->eval(binding);
            switch (RDF_OID::get_generic_sub_type(expr_oid)) {
            case RDF_OID::GenericSubType::STRING_SIMPLE: {
                auto str = Conversions::unpack_string(expr_oid);
                res += str;
                status = ConcatStatus::SIMPLE;
                break;
            }
            case RDF_OID::GenericSubType::STRING_XSD: {
                auto str = Conversions::unpack_string(expr_oid);
                res += str;
                if (status == ConcatStatus::UNSET || status == ConcatStatus::XSD) {
                    status = ConcatStatus::XSD;
                } else {
                    status = ConcatStatus::SIMPLE;
                }
                break;
            }
            case RDF_OID::GenericSubType::STRING_LANG: {
                auto&& [lang, str] = Conversions::unpack_string_lang(expr_oid);
                res += str;
                if (status == ConcatStatus::UNSET) {
                    res_lang = lang;
                    status = ConcatStatus::LANG;
                } else if (status != ConcatStatus::LANG || lang != res_lang) {
                    status = ConcatStatus::SIMPLE;
                }
                break;
            }
            default:
                return ObjectId::get_null();
            }
        }

        switch (status) {
        case ConcatStatus::UNSET:
        case ConcatStatus::SIMPLE:
            return Conversions::pack_string_simple(res);
        case ConcatStatus::XSD:
            return Conversions::pack_string_xsd(res);
        case ConcatStatus::LANG:
            return Conversions::pack_string_lang(res_lang, res);
        default:
            assert(false);
            return ObjectId::get_null();
        }
    }

    void accept_visitor(BindingExprVisitor& visitor) override {
        visitor.visit(*this);
    }
};
} // namespace SPARQL
