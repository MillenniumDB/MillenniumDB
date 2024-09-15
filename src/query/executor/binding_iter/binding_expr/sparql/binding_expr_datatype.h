#pragma once

#include <cassert>
#include <memory>
#include <sstream>

#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace SPARQL {
class BindingExprDatatype : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    BindingExprDatatype(std::unique_ptr<BindingExpr> expr) :
        expr (std::move(expr)) { }

    ObjectId eval(const Binding& binding) override {
        auto expr_oid = expr->eval(binding);

        switch (RDF_OID::get_generic_sub_type(expr_oid)) {
        case RDF_OID::GenericSubType::STRING_DATATYPE: {
            auto&& [datatype, str] = Conversions::unpack_string_datatype(expr_oid);
            return Conversions::pack_iri(datatype);
        }
        case RDF_OID::GenericSubType::STRING_SIMPLE:
        case RDF_OID::GenericSubType::STRING_XSD:
            return Conversions::pack_iri("http://www.w3.org/2001/XMLSchema#string");
        case RDF_OID::GenericSubType::STRING_LANG:
            return Conversions::pack_iri("http://www.w3.org/1999/02/22-rdf-syntax-ns#langString");
        case RDF_OID::GenericSubType::INTEGER:
            return Conversions::pack_iri("http://www.w3.org/2001/XMLSchema#integer");
        case RDF_OID::GenericSubType::DECIMAL:
            return Conversions::pack_iri("http://www.w3.org/2001/XMLSchema#decimal");
        case RDF_OID::GenericSubType::FLOAT:
            return Conversions::pack_iri("http://www.w3.org/2001/XMLSchema#float");
        case RDF_OID::GenericSubType::DOUBLE:
            return Conversions::pack_iri("http://www.w3.org/2001/XMLSchema#double");
        case RDF_OID::GenericSubType::DATE: {
            switch (RDF_OID::get_type(expr_oid)) {
            case RDF_OID::Type::DATE:
                return Conversions::pack_iri("http://www.w3.org/2001/XMLSchema#date");
            case RDF_OID::Type::DATETIME:
                return Conversions::pack_iri("http://www.w3.org/2001/XMLSchema#dateTime");
            case RDF_OID::Type::TIME:
                return Conversions::pack_iri("http://www.w3.org/2001/XMLSchema#time");
            case RDF_OID::Type::DATETIMESTAMP:
                return Conversions::pack_iri("http://www.w3.org/2001/XMLSchema#dateTimeStamp");
            default: {
                assert(false);
                return ObjectId::get_null();
            }
            }
        }
        case RDF_OID::GenericSubType::BOOL:
            return Conversions::pack_iri("http://www.w3.org/2001/XMLSchema#boolean");
        default:
            return ObjectId::get_null();
        }
    }

    void accept_visitor(BindingExprVisitor& visitor) override {
        visitor.visit(*this);
    }
};
} // namespace SPARQL