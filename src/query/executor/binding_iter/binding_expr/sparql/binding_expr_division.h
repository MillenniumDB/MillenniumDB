#pragma once

#include <memory>

#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace SPARQL {
class BindingExprDivision : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> lhs;
    std::unique_ptr<BindingExpr> rhs;

    BindingExprDivision(std::unique_ptr<BindingExpr> lhs, std::unique_ptr<BindingExpr> rhs) :
        lhs(std::move(lhs)),
        rhs(std::move(rhs))
    { }

    template<typename T>
    inline ObjectId handle_divide_tensors(ObjectId lhs_oid, ObjectId rhs_oid)
    {
        bool swapped = false;
        if (RDF_OID::get_generic_type(rhs_oid) == RDF_OID::GenericType::TENSOR) {
            // Make sure that a tensor will be on the left
            std::swap(lhs_oid, rhs_oid);
            swapped = true;
        }

        auto lhs = Conversions::to_tensor<T>(lhs_oid);
        if (RDF_OID::get_generic_type(rhs_oid) == RDF_OID::GenericType::TENSOR) {
            // Both tensors
            auto rhs = Conversions::to_tensor<T>(rhs_oid);
            if (lhs.size() != rhs.size()) {
                return ObjectId::get_null();
            }
            if (swapped) {
                return Conversions::pack_tensor<T>(rhs.divide(lhs));
            } else {
                return Conversions::pack_tensor<T>(lhs.divide(rhs));
            }
        } else {
            // lhs tensor, rhs scalar
            if constexpr (std::is_same_v<T, float>) {
                const auto rhs = Conversions::to_float(rhs_oid);
                if (swapped) {
                    return Conversions::pack_tensor<T>(lhs.multiplicative_inverse().multiply(rhs));
                } else {
                    return Conversions::pack_tensor<T>(lhs.divide(rhs));
                }
            } else if constexpr (std::is_same_v<T, double>) {
                const auto rhs = Conversions::to_double(rhs_oid);
                if (swapped) {
                    return Conversions::pack_tensor<T>(lhs.multiplicative_inverse().multiply(rhs));
                } else {
                    return Conversions::pack_tensor<T>(lhs.divide(rhs));
                }
            } else {
                assert(false && "Unhandled tensor type");
                return ObjectId::get_null();
            }
        }

        if (swapped) {
            // Due the swap, we evaluated (lhs - rhs) as (rhs - lhs), so the result must be negated
            lhs.negate();
        }
        return Conversions::pack_tensor<T>(lhs);
    }

    ObjectId eval(const Binding& binding) override
    {
        auto lhs_oid = lhs->eval(binding);
        auto rhs_oid = rhs->eval(binding);

        auto optype = Conversions::calculate_optype(lhs_oid, rhs_oid);

        switch (optype) {
        case Conversions::OPTYPE_INTEGER: {
            // In SPARQL-1.1 integer division is done using decimals, and the result is a decimal
            auto rhs = Conversions::to_decimal(rhs_oid);
            if (rhs == 0) {
                return ObjectId::get_null();
            }
            auto lhs = Conversions::to_decimal(lhs_oid);
            return Conversions::pack_decimal(lhs / rhs);
        }
        case Conversions::OPTYPE_DECIMAL: {
            auto rhs = Conversions::to_decimal(rhs_oid);
            if (rhs == 0) {
                return ObjectId::get_null();
            }
            auto lhs = Conversions::to_decimal(lhs_oid);
            return Conversions::pack_decimal(lhs / rhs);
        }
        case Conversions::OPTYPE_FLOAT: {
            auto lhs = Conversions::to_float(lhs_oid);
            auto rhs = Conversions::to_float(rhs_oid);
            // Division by zero, etc is handle by the floating point implementation.
            // SPARQL 1.1 follows IEEE 754-2008.
            return Conversions::pack_float(lhs / rhs);
        }
        case Conversions::OPTYPE_DOUBLE: {
            auto lhs = Conversions::to_double(lhs_oid);
            auto rhs = Conversions::to_double(rhs_oid);
            // Division by zero, etc is handle by the floating point implementation.
            // SPARQL 1.1 follows IEEE 754-2008.
            return Conversions::pack_double(lhs / rhs);
        }
        case Conversions::OPTYPE_TENSOR_FLOAT: {
            return handle_divide_tensors<float>(lhs_oid, rhs_oid);
        }
        case Conversions::OPTYPE_TENSOR_DOUBLE: {
            return handle_divide_tensors<double>(lhs_oid, rhs_oid);
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

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }
};
} // namespace SPARQL
