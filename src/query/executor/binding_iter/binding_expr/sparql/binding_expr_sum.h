#pragma once

#include <memory>

#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

// IMPORTANT: This is not the aggregate SUM(), its the function mdbfn:sum() for tensors
namespace SPARQL {
class BindingExprSum : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    BindingExprSum(std::unique_ptr<BindingExpr> expr) :
        expr(std::move(expr))
    { }

    ObjectId eval(const Binding& binding) override
    {
        const auto expr_oid = expr->eval(binding);

        switch (RDF_OID::get_generic_sub_type(expr_oid)) {
        case RDF_OID::GenericSubType::TENSOR_FLOAT: {
            auto tensor = Conversions::unpack_tensor<float>(expr_oid);
            return Conversions::pack_float(tensor.sum());
        }
        case RDF_OID::GenericSubType::TENSOR_DOUBLE: {
            auto tensor = Conversions::unpack_tensor<double>(expr_oid);
            return Conversions::pack_double(tensor.sum());
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
        os << "mdbfn:sum(";
        expr->print(os, ops);
        os << ')';
    }
};
} // namespace SPARQL
