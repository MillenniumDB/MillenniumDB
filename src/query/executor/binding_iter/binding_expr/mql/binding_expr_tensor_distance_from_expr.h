#pragma once

#include <memory>

#include "graph_models/common/conversions.h"
#include "query/exceptions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"
#include "storage/index/tensor_store/metric.h"
#include "storage/index/tensor_store/tensor_store.h"
#include "storage/index/tensor_store/tensor_store_manager.h"
#include "storage/index/tensor_store/versioned_tensor.h"

namespace MQL {
class BindingExprTensorDistanceFromExpr : public BindingExpr {
public:
    Metric::MetricFuncType* metric_function;

    TensorStore* tensor_store;

    Metric::MetricType metric_type;

    std::unique_ptr<BindingExpr> expr1;
    std::unique_ptr<BindingExpr> expr2;

    BindingExprTensorDistanceFromExpr(
        const std::string& tensor_store_name,
        Metric::MetricType metric_type,
        std::unique_ptr<BindingExpr> expr1,
        std::unique_ptr<BindingExpr> expr2
    ) :
        metric_type(metric_type),
        expr1(std::move(expr1)),
        expr2(std::move(expr2))
    {
        const bool found = tensor_store_manager.get_tensor_store(tensor_store_name, &tensor_store);
        if (!found) {
            throw QuerySemanticException("TensorStore \"" + tensor_store_name + "\" does not exist");
        }
        metric_function = Metric::get_metric_function(metric_type);
    }

    ObjectId eval(const Binding& binding) override
    {
        auto expr1_oid = expr1->eval(binding);
        auto expr2_oid = expr2->eval(binding);

        VTensor* vtensor1;
        if (!tensor_store->get(expr1_oid, &vtensor1)) {
            return ObjectId::get_null();
        }

        VTensor* vtensor2;
        if (!tensor_store->get(expr2_oid, &vtensor2)) {
            tensor_store->unpin(*vtensor1);
            return ObjectId::get_null();
        }

        const float distance = metric_function(
            vtensor1->get_tensor(),
            vtensor2->get_tensor(),
            tensor_store->tensors_dim()
        );
        tensor_store->unpin(*vtensor1);
        tensor_store->unpin(*vtensor2);

        return Common::Conversions::pack_float(distance);
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }
};
} // namespace MQL
