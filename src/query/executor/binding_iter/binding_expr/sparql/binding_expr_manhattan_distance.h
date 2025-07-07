#pragma once

#include <cassert>
#include <memory>

#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace SPARQL {
class BindingExprManhattanDistance : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> lhs;
    std::unique_ptr<BindingExpr> rhs;

    BindingExprManhattanDistance(std::unique_ptr<BindingExpr> lhs, std::unique_ptr<BindingExpr> rhs) :
        lhs(std::move(lhs)),
        rhs(std::move(rhs))
    { }

    ObjectId eval(const Binding& binding) override
    {
        // This operator is only defined for tensors
        const auto lhs_oid = lhs->eval(binding);
        const auto rhs_oid = rhs->eval(binding);

        const auto lhs_gen_t = RDF_OID::get_generic_type(lhs_oid);
        if (lhs_gen_t != RDF_OID::GenericType::TENSOR) {
            return ObjectId::get_null();
        }

        const auto rhs_gen_t = RDF_OID::get_generic_type(rhs_oid);
        if (lhs_gen_t != rhs_gen_t) {
            return ObjectId::get_null();
        }

        const auto optype = Conversions::calculate_optype(lhs_oid, rhs_oid);
        switch (optype) {
        case Conversions::OpType::TENSOR_FLOAT: {
            const auto lhs = Conversions::to_tensor<float>(lhs_oid);
            const auto rhs = Conversions::to_tensor<float>(rhs_oid);
            if (lhs.size() != rhs.size()) {
                return ObjectId::get_null();
            }
            return Conversions::pack_float(lhs.manhattan_distance(rhs));
        }
        case Conversions::OpType::TENSOR_DOUBLE: {
            const auto lhs = Conversions::to_tensor<double>(lhs_oid);
            const auto rhs = Conversions::to_tensor<double>(rhs_oid);
            if (lhs.size() != rhs.size()) {
                return ObjectId::get_null();
            }
            return Conversions::pack_double(lhs.manhattan_distance(rhs));
        }
        case Conversions::OpType::INTEGER:
        case Conversions::OpType::DECIMAL:
        case Conversions::OpType::FLOAT:
        case Conversions::OpType::DOUBLE:
        case Conversions::OpType::INVALID:
            return ObjectId::get_null();
        default:
            throw LogicException("This should never happen");
        }
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }
};
} // namespace SPARQL
