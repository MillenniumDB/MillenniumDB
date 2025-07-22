#pragma once

#include <memory>

#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace SPARQL {
class BindingExprStr : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    BindingExprStr(std::unique_ptr<BindingExpr> expr) :
        expr(std::move(expr))
    { }

    ObjectId eval(const Binding& binding) override
    {
        auto expr_oid = expr->eval(binding);

        switch (RDF_OID::get_generic_sub_type(expr_oid)) {
        case RDF_OID::GenericSubType::BLANK:
        case RDF_OID::GenericSubType::IRI:
        case RDF_OID::GenericSubType::STRING_SIMPLE:
        case RDF_OID::GenericSubType::STRING_XSD:
        case RDF_OID::GenericSubType::STRING_LANG:
        case RDF_OID::GenericSubType::STRING_DATATYPE:
        case RDF_OID::GenericSubType::INTEGER:
        case RDF_OID::GenericSubType::FLOAT:
        case RDF_OID::GenericSubType::DOUBLE:
        case RDF_OID::GenericSubType::DECIMAL:
        case RDF_OID::GenericSubType::BOOL:
        case RDF_OID::GenericSubType::DATE: {
            auto str = Conversions::to_lexical_str(expr_oid);
            return Conversions::pack_string_simple(str);
        }
        case RDF_OID::GenericSubType::NULL_ID:
        case RDF_OID::GenericSubType::PATH:
            return ObjectId::get_null();
        default:
            assert(false);
            return ObjectId::get_null();
        }
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    void print(std::ostream& os, std::vector<BindingIter*> ops) const override
    {
        os << "STR(";
        expr->print(os, ops);
        os << ')';
    }
};
} // namespace SPARQL
