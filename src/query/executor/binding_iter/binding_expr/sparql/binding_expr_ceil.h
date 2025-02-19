#pragma once

#include <cmath>
#include <memory>

#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace SPARQL {
class BindingExprCeil : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    BindingExprCeil(std::unique_ptr<BindingExpr> expr) : expr(std::move(expr)) { }

    ObjectId eval(const Binding& binding) override {
        auto expr_oid = expr->eval(binding);

        switch (RDF_OID::get_generic_sub_type(expr_oid)) {
        case RDF_OID::GenericSubType::INTEGER: {
            auto n = Conversions::unpack_int(expr_oid);
            return ObjectId(Conversions::pack_int(std::ceil(n)));
        }
        case RDF_OID::GenericSubType::DECIMAL: {
            auto n = Conversions::unpack_decimal(expr_oid);
            return ObjectId(Conversions::pack_decimal(n.ceil()));
        }
        case RDF_OID::GenericSubType::FLOAT: {
            auto n = Conversions::unpack_float(expr_oid);
            return ObjectId(Conversions::pack_float(std::ceil(n)));
        }
        case RDF_OID::GenericSubType::DOUBLE: {
            auto n = Conversions::unpack_double(expr_oid);
            return ObjectId(Conversions::pack_double(std::ceil(n)));
        }
        case RDF_OID::GenericSubType::TENSOR_FLOAT: {
            auto tensor = Conversions::unpack_tensor<float>(expr_oid);
            return ObjectId(Conversions::pack_tensor<float>(tensor.ceil()));
        }
        case RDF_OID::GenericSubType::TENSOR_DOUBLE: {
            auto tensor = Conversions::unpack_tensor<double>(expr_oid);
            return ObjectId(Conversions::pack_tensor<double>(tensor.ceil()));
        }
        default:
            return ObjectId::get_null();
        }
    }

    void accept_visitor(BindingExprVisitor& visitor) override {
        visitor.visit(*this);
    }
};
} // namespace SPARQL
