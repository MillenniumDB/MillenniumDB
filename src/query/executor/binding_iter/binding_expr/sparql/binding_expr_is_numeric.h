#pragma once

#include <memory>

#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace SPARQL {
class BindingExprIsNumeric : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    BindingExprIsNumeric(std::unique_ptr<BindingExpr> expr) :
        expr(std::move(expr))
    { }

    ObjectId eval(const Binding& binding) override
    {
        auto expr_oid = expr->eval(binding);

        switch (RDF_OID::get_generic_sub_type(expr_oid)) {
        case RDF_OID::GenericSubType::INTEGER:
        case RDF_OID::GenericSubType::DECIMAL:
        case RDF_OID::GenericSubType::FLOAT:
        case RDF_OID::GenericSubType::DOUBLE: {
            return Conversions::pack_bool(true);
        }
        case RDF_OID::GenericSubType::NULL_ID:
            return ObjectId::get_null();
        default:
            return Conversions::pack_bool(false);
        }
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    void print(std::ostream& os, std::vector<BindingIter*> ops) const override
    {
        os << "isNUMERIC(";
        expr->print(os, ops);
        os << ')';
    }
};
} // namespace SPARQL
