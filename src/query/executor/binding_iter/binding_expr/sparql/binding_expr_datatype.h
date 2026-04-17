#pragma once

#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"
#include "query/parser/grammar/sparql/mdb_extensions.h"

#include <cassert>
#include <memory>

namespace SPARQL {
class BindingExprDatatype : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    BindingExprDatatype(std::unique_ptr<BindingExpr> expr) :
        expr(std::move(expr))
    { }

    ObjectId eval(const Binding& binding) override
    {
        auto expr_oid = expr->eval(binding);

        switch (expr_oid.subtype()) {
        case ObjectSubType::StringDatatype: {
            auto&& [datatype, str] = Conversions::unpack_string_datatype(expr_oid);
            return Conversions::pack_iri(datatype);
        }
        case ObjectSubType::String:
        case ObjectSubType::StringXsd:
            return Conversions::pack_iri("http://www.w3.org/2001/XMLSchema#string");
        case ObjectSubType::StringLang:
            return Conversions::pack_iri("http://www.w3.org/1999/02/22-rdf-syntax-ns#langString");
        case ObjectSubType::Int:
            return Conversions::pack_iri("http://www.w3.org/2001/XMLSchema#integer");
        case ObjectSubType::Decimal:
            return Conversions::pack_iri("http://www.w3.org/2001/XMLSchema#decimal");
        case ObjectSubType::Float:
            return Conversions::pack_iri("http://www.w3.org/2001/XMLSchema#float");
        case ObjectSubType::Double:
            return Conversions::pack_iri("http://www.w3.org/2001/XMLSchema#double");
        case ObjectSubType::TemporalLiteral: {
            switch (expr_oid.type()) {
                case ObjectType::Date:
                    return Conversions::pack_iri("http://www.w3.org/2001/XMLSchema#date");
                case ObjectType::Datetime:
                    return Conversions::pack_iri("http://www.w3.org/2001/XMLSchema#dateTime");
                case ObjectType::Datetimestamp:
                    return Conversions::pack_iri("http://www.w3.org/2001/XMLSchema#dateTimeStamp");
                case ObjectType::Time:
                    return Conversions::pack_iri("http://www.w3.org/2001/XMLSchema#time");
                default:
                    assert(false);
                    return ObjectId::get_null();
            }
        }
        case ObjectSubType::Bool:
            return Conversions::pack_iri("http://www.w3.org/2001/XMLSchema#boolean");
        case ObjectSubType::TensorFloat:
            return Conversions::pack_iri(MDBExtensions::Type::TENSOR_FLOAT_IRI);
        case ObjectSubType::TensorDouble:
            return Conversions::pack_iri(MDBExtensions::Type::TENSOR_DOUBLE_IRI);
        default:
            return ObjectId::get_null();
        }
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    void print(std::ostream& os, std::vector<BindingIter*>& ops) const override
    {
        os << "DATATYPE(";
        expr->print(os, ops);
        os << ')';
    }
};
} // namespace SPARQL
