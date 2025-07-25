#pragma once

#include <memory>

#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace SPARQL {
class BindingExprUnaryMinus : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    BindingExprUnaryMinus(std::unique_ptr<BindingExpr> expr) :
        expr(std::move(expr))
    { }

    ObjectId eval(const Binding& binding) override
    {
        auto expr_oid = expr->eval(binding);

        switch (RDF_OID::get_generic_sub_type(expr_oid)) {
        case RDF_OID::GenericSubType::INTEGER: {
            auto i = Conversions::unpack_int(expr_oid);
            return Conversions::pack_int(i * -1);
        }
        case RDF_OID::GenericSubType::DECIMAL: {
            auto d = Conversions::unpack_decimal(expr_oid);
            return Conversions::pack_decimal(-d);
        }
        case RDF_OID::GenericSubType::FLOAT: {
            auto f = Conversions::unpack_float(expr_oid);
            return Conversions::pack_float(f * -1);
        }
        case RDF_OID::GenericSubType::DOUBLE: {
            auto d = Conversions::unpack_double(expr_oid);
            return Conversions::pack_double(d * -1);
        }
        case RDF_OID::GenericSubType::TENSOR_FLOAT: {
            auto tensor = Conversions::unpack_tensor<float>(expr_oid);
            tensor.negate();
            return Conversions::pack_tensor<float>(tensor);
        }
        case RDF_OID::GenericSubType::TENSOR_DOUBLE: {
            auto tensor = Conversions::unpack_tensor<double>(expr_oid);
            tensor.negate();
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

    void print(std::ostream& os, std::vector<BindingIter*> ops) const override
    {
        os << "-(";
        expr->print(os, ops);
        os << ')';
    }
};
} // namespace SPARQL
