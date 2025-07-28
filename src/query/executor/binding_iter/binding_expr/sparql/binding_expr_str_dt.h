#pragma once

#include <memory>

#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace SPARQL {
class BindingExprStrDT : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> lhs;
    std::unique_ptr<BindingExpr> rhs;

    BindingExprStrDT(std::unique_ptr<BindingExpr> lhs, std::unique_ptr<BindingExpr> rhs) :
        lhs(std::move(lhs)),
        rhs(std::move(rhs))
    { }

    ObjectId eval(const Binding& binding) override
    {
        auto lhs_oid = lhs->eval(binding);
        auto rhs_oid = rhs->eval(binding);

        auto lhs_subtype = RDF_OID::get_generic_sub_type(lhs_oid);
        if (lhs_subtype != RDF_OID::GenericSubType::STRING_SIMPLE
            && lhs_subtype != RDF_OID::GenericSubType::STRING_XSD)
        {
            return ObjectId::get_null();
        }

        auto expr2_subtype = RDF_OID::get_generic_sub_type(rhs_oid);
        if (expr2_subtype != RDF_OID::GenericSubType::IRI) {
            return ObjectId::get_null();
        }

        auto str = Conversions::to_lexical_str(lhs_oid);
        auto dt = Conversions::unpack_iri(rhs_oid);

        return Conversions::try_pack_string_datatype(dt, str);
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    void print(std::ostream& os, std::vector<BindingIter*> ops) const override
    {
        os << "STRDT(";
        lhs->print(os, ops);
        os << ", ";
        rhs->print(os, ops);
        os << ')';
    }
};
} // namespace SPARQL
