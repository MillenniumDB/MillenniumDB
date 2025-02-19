#pragma once

#include <cmath>
#include <memory>

#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace SPARQL {
class BindingExprPow : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> lhs;
    std::unique_ptr<BindingExpr> rhs;

    BindingExprPow(std::unique_ptr<BindingExpr> lhs, std::unique_ptr<BindingExpr> rhs) :
        lhs(std::move(lhs)),
        rhs(std::move(rhs))
    { }

    template<typename T>
    inline ObjectId handle_pow_tensors(ObjectId lhs_oid, ObjectId rhs_oid)
    {
        if (RDF_OID::get_generic_type(lhs_oid) != RDF_OID::GenericType::TENSOR
            || RDF_OID::get_generic_type(rhs_oid) != RDF_OID::GenericType::NUMERIC)
        {
            // Only tensor to the power of a scalar is allowed
            return ObjectId::get_null();
        }

        auto lhs = Conversions::to_tensor<T>(lhs_oid);
        if constexpr (std::is_same_v<T, float>) {
            const auto rhs = Conversions::to_float(rhs_oid);
            return Conversions::pack_tensor<T>(lhs.pow(rhs));
        } else if constexpr (std::is_same_v<T, double>) {
            const auto rhs = Conversions::to_double(rhs_oid);
            return Conversions::pack_tensor<T>(lhs.pow(rhs));
        } else {
            assert(false && "Unhandled tensor type");
            return ObjectId::get_null();
        }
    }

    ObjectId eval(const Binding& binding) override
    {
        auto lhs_oid = lhs->eval(binding);
        auto rhs_oid = rhs->eval(binding);

        auto optype = Conversions::calculate_optype(lhs_oid, rhs_oid);

        switch (optype) {
        case Conversions::OPTYPE_INTEGER: {
            auto lhs = Conversions::to_double(lhs_oid);
            auto rhs = Conversions::to_double(rhs_oid);
            return Conversions::pack_double(std::pow(lhs, rhs));
        }
        case Conversions::OPTYPE_DECIMAL: {
            auto lhs = Conversions::to_decimal(lhs_oid);
            auto rhs = Conversions::to_decimal(rhs_oid);
            return Conversions::pack_decimal(lhs.pow(rhs));
        }
        case Conversions::OPTYPE_FLOAT: {
            auto lhs = Conversions::to_float(lhs_oid);
            auto rhs = Conversions::to_float(rhs_oid);
            return Conversions::pack_float(std::pow(lhs, rhs));
        }
        case Conversions::OPTYPE_DOUBLE: {
            auto lhs = Conversions::to_double(lhs_oid);
            auto rhs = Conversions::to_double(rhs_oid);
            return Conversions::pack_double(std::pow(lhs, rhs));
        }
        case Conversions::OPTYPE_TENSOR_FLOAT: {
            return handle_pow_tensors<float>(lhs_oid, rhs_oid);
        }
        case Conversions::OPTYPE_TENSOR_DOUBLE: {
            return handle_pow_tensors<double>(lhs_oid, rhs_oid);
        }
        case Conversions::OPTYPE_INVALID: {
            return ObjectId::get_null();
        }
        default:
            assert(false);
            return ObjectId::get_null();
        }
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }
};
} // namespace SPARQL
