#pragma once

#include <cmath>
#include <memory>

#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace SPARQL {
class BindingExprSqrt : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    BindingExprSqrt(std::unique_ptr<BindingExpr> expr) :
        expr(std::move(expr))
    { }

    ObjectId eval(const Binding& binding) override
    {
        auto expr_oid = expr->eval(binding);

        switch (RDF_OID::get_generic_sub_type(expr_oid)) {
        case RDF_OID::GenericSubType::INTEGER:
        case RDF_OID::GenericSubType::DECIMAL:
        case RDF_OID::GenericSubType::FLOAT: {
            auto f = Conversions::unpack_float(expr_oid);
            return Conversions::pack_float(std::sqrt(f));
        }
        case RDF_OID::GenericSubType::DOUBLE: {
            auto d = Conversions::to_double(expr_oid);
            return Conversions::pack_double(std::sqrt(d));
        }
        case RDF_OID::GenericSubType::TENSOR_FLOAT: {
            auto tensor = Conversions::unpack_tensor<float>(expr_oid);
            tensor.sqrt();
            return Conversions::pack_tensor<float>(tensor);
        }
        case RDF_OID::GenericSubType::TENSOR_DOUBLE: {
            auto tensor = Conversions::unpack_tensor<double>(expr_oid);
            tensor.sqrt();
            return Conversions::pack_tensor<double>(tensor);
        }
        default:
            return ObjectId::get_null();
        }
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }
};
} // namespace SPARQL
