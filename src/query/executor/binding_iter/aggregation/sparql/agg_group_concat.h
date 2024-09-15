#pragma once

#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/aggregation/agg.h"
#include "query/executor/binding_iter/binding_expr/sparql_binding_expr_printer.h"

namespace SPARQL {

enum class GroupConcatType {
    UNSET,
    NULL_RESULT,
    SIMPLE,
    XSD,
    LANG,
};

class AggGroupConcat : public Agg {
public:
    using Agg::Agg;

    AggGroupConcat(VarId var_id, std::unique_ptr<BindingExpr> expr, const std::string& sep) :
        Agg (var_id, std::move(expr)),
        sep (std::move(sep)) { }

    void begin() override {
        res = std::string();
        type = GroupConcatType::UNSET;
    }

    void process() override {
        if (type == GroupConcatType::NULL_RESULT) {
            return;
        }

        if (type != GroupConcatType::UNSET) {
            res += sep;
        }

        auto oid = expr->eval(*binding);

        // at this point type may be UNSET, SIMPLE, XSD or LANG
        switch (RDF_OID::get_generic_sub_type(oid)) {
        case RDF_OID::GenericSubType::STRING_SIMPLE: {
            auto str = Conversions::unpack_string(oid);
            res += str;
            type = GroupConcatType::SIMPLE;
            break;
        }
        case RDF_OID::GenericSubType::STRING_XSD: {
            auto str = Conversions::unpack_string(oid);
            res += str;
            if (type == GroupConcatType::UNSET) {
                type = GroupConcatType::XSD;
            } else if (type != GroupConcatType::XSD) {
                type = GroupConcatType::SIMPLE;
            }
            // else type remains GroupConcatType::XSD
            break;
        }
        case RDF_OID::GenericSubType::STRING_LANG: {
            auto&& [l, str] = Conversions::unpack_string_lang(oid);
            res += str;
            if (type == GroupConcatType::UNSET) {
                lang = l;
                type = GroupConcatType::LANG;
            } else if (type == GroupConcatType::LANG) {
                if (lang != l) {
                    type = GroupConcatType::SIMPLE;
                }
            } else { // XSD or SIMPLE
                type = GroupConcatType::SIMPLE;
            }
            break;
        }
        default: {
            type = GroupConcatType::NULL_RESULT;
            break;
        }
        }
    }

    // indicates the end of a group
    ObjectId get() override {
        switch (type) {
        case GroupConcatType::UNSET:
            return Conversions::pack_empty_string();
        case GroupConcatType::NULL_RESULT:
            return ObjectId::get_null();
        case GroupConcatType::SIMPLE:
            return Conversions::pack_string_simple(res);
        case GroupConcatType::XSD:
            return Conversions::pack_string_xsd(res);
        case GroupConcatType::LANG:
            return Conversions::pack_string_lang(lang, res);
        default:
            assert(false);
            return ObjectId::get_null();
        }
    }

    std::ostream& print_to_ostream(std::ostream& os) const override {
        os << "GROUPCONCAT(";
        BindingExprPrinter printer(os);
        expr->accept_visitor(printer);
        os << ")";
        return os;
    }

    std::string sep;

private:
    GroupConcatType type;
    std::string res;
    std::string lang;

};
} // namespace SPARQL
