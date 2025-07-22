#pragma once

#include <memory>
#include <string>

#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace SPARQL {
class BindingExprContains : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> lhs;
    std::unique_ptr<BindingExpr> rhs;

    BindingExprContains(std::unique_ptr<BindingExpr> lhs, std::unique_ptr<BindingExpr> rhs) :
        lhs(std::move(lhs)),
        rhs(std::move(rhs))
    { }

    ObjectId eval(const Binding& binding) override
    {
        auto lhs_oid = lhs->eval(binding);
        auto rhs_oid = rhs->eval(binding);

        auto lhs_sub = RDF_OID::get_generic_sub_type(lhs_oid);
        auto rhs_sub = RDF_OID::get_generic_sub_type(rhs_oid);

        if (lhs_sub != RDF_OID::GenericSubType::STRING_SIMPLE
            && lhs_sub != RDF_OID::GenericSubType::STRING_XSD
            && lhs_sub != RDF_OID::GenericSubType::STRING_LANG)
        {
            return ObjectId::get_null();
        }

        if (rhs_sub != RDF_OID::GenericSubType::STRING_SIMPLE
            && rhs_sub != RDF_OID::GenericSubType::STRING_XSD
            && rhs_sub != RDF_OID::GenericSubType::STRING_LANG)
        {
            return ObjectId::get_null();
        }

        if (lhs_sub != RDF_OID::GenericSubType::STRING_LANG
            && rhs_sub == RDF_OID::GenericSubType::STRING_LANG)
        {
            return ObjectId::get_null();
        }

        std::string lhs_str;
        std::string rhs_str;

        if (lhs_sub == RDF_OID::GenericSubType::STRING_LANG
            && rhs_sub == RDF_OID::GenericSubType::STRING_LANG)
        {
            auto [lhs_l, lhs_s] = Conversions::unpack_string_lang(lhs_oid);
            auto [rhs_l, rhs_s] = Conversions::unpack_string_lang(rhs_oid);
            if (lhs_l != rhs_l) {
                return ObjectId::get_null();
            }
            lhs_str = std::move(lhs_s);
            rhs_str = std::move(rhs_s);
        } else {
            lhs_str = Conversions::to_lexical_str(lhs_oid);
            rhs_str = Conversions::to_lexical_str(rhs_oid);
        }

        if (rhs_str.size() == 0) {
            return SPARQL::Conversions::pack_bool(true);
        }
        if (lhs_str.size() == 0) {
            return SPARQL::Conversions::pack_bool(false);
        }

        auto it = lhs_str.find(rhs_str);
        return SPARQL::Conversions::pack_bool(it != std::string::npos);
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    void print(std::ostream& os, std::vector<BindingIter*> ops) const override
    {
        os << "CONTAINS(";
        lhs->print(os, ops);
        os << ", ";
        rhs->print(os, ops);
        os << ')';
    }
};
} // namespace SPARQL
