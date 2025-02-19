#pragma once

#include <memory>

#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace SPARQL {
class BindingExprMultiplication : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> lhs;
    std::unique_ptr<BindingExpr> rhs;

    BindingExprMultiplication(std::unique_ptr<BindingExpr> lhs, std::unique_ptr<BindingExpr> rhs) :
        lhs(std::move(lhs)), rhs(std::move(rhs)) { }

    template<typename T>
    inline ObjectId handle_multiply_tensors(ObjectId lhs_oid, ObjectId rhs_oid)
    {
        if (RDF_OID::get_generic_type(rhs_oid) == RDF_OID::GenericType::TENSOR) {
            // Make sure that a tensor will be on the left
            std::swap(lhs_oid, rhs_oid);
        }

        auto lhs = Conversions::to_tensor<T>(lhs_oid);
        if (RDF_OID::get_generic_type(rhs_oid) == RDF_OID::GenericType::TENSOR) {
            // Both tensors
            const auto rhs = Conversions::to_tensor<T>(rhs_oid);
            if (lhs.size() != rhs.size()) {
                return ObjectId::get_null();
            }
            return Conversions::pack_tensor<T>(lhs.multiply(rhs));
        } else {
            // lhs tensor, rhs scalar
            if constexpr (std::is_same_v<T, float>) {
                const auto rhs = Conversions::to_float(rhs_oid);
                return Conversions::pack_tensor<T>(lhs.multiply(rhs));
            } else if constexpr (std::is_same_v<T, double>) {
                const auto rhs = Conversions::to_double(rhs_oid);
                return Conversions::pack_tensor<T>(lhs.multiply(rhs));
            } else {
                assert(false && "Unhandled tensor type");
                return ObjectId::get_null();
            }
        }
    }

    ObjectId eval(const Binding& binding) override {
        auto lhs_oid = lhs->eval(binding);
        auto rhs_oid = rhs->eval(binding);

        auto optype = Conversions::calculate_optype(lhs_oid, rhs_oid);

        switch (optype) {
        case Conversions::OPTYPE_INTEGER: {
            auto lhs = Conversions::to_integer(lhs_oid);
            auto rhs = Conversions::to_integer(rhs_oid);
            return Conversions::pack_int(lhs * rhs);
        }
        case Conversions::OPTYPE_DECIMAL: {
            auto lhs = Conversions::to_decimal(lhs_oid);
            auto rhs = Conversions::to_decimal(rhs_oid);
            return Conversions::pack_decimal(lhs * rhs);
        }
        case Conversions::OPTYPE_FLOAT: {
            auto lhs = Conversions::to_float(lhs_oid);
            auto rhs = Conversions::to_float(rhs_oid);
            return Conversions::pack_float(lhs * rhs);
        }
        case Conversions::OPTYPE_DOUBLE: {
            auto lhs = Conversions::to_double(lhs_oid);
            auto rhs = Conversions::to_double(rhs_oid);
            return Conversions::pack_double(lhs * rhs);
        }
        case Conversions::OPTYPE_TENSOR_FLOAT: {
            return handle_multiply_tensors<float>(lhs_oid, rhs_oid);
        }
        case Conversions::OPTYPE_TENSOR_DOUBLE: {
            return handle_multiply_tensors<double>(lhs_oid, rhs_oid);
        }
        case Conversions::OPTYPE_INVALID: {
            return ObjectId::get_null();
        }
        default: {
            assert(false);
            return ObjectId::get_null();
        }
        }
    }

    void accept_visitor(BindingExprVisitor& visitor) override {
        visitor.visit(*this);
    }
};
} // namespace SPARQL
