#pragma once

#include <memory>

#include "graph_models/common/conversions.h"
#include "graph_models/quad_model/quad_model.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"
#include "storage/index/tensor_store/metric.h"
#include "storage/index/tensor_store/tensor_store.h"
#include "storage/index/tensor_store/tensor_store_manager.h"
#include "storage/index/tensor_store/versioned_tensor.h"

namespace MQL {
class BindingExprTensorDistance : public BindingExpr {
public:
    Metric::MetricFuncType* metric_function;

    TensorStore* tensor_store;

    Metric::MetricType metric_type;

    std::unique_ptr<BindingExpr> lhs_expr;
    std::unique_ptr<BindingExpr> rhs_expr;

    BindingExprTensorDistance(
        const std::string& tensor_store_name,
        Metric::MetricType metric_type,
        std::unique_ptr<BindingExpr> lhs_expr_,
        std::unique_ptr<BindingExpr> rhs_expr_
    ) :
        metric_type(metric_type),
        lhs_expr(std::move(lhs_expr_)),
        rhs_expr(std::move(rhs_expr_))
    {
        auto& tensor_store_manager = quad_model.catalog.tensor_store_manager;
        [[maybe_unused]] const bool found = tensor_store_manager.get_tensor_store(
            tensor_store_name,
            &tensor_store
        );
        assert(found && "TensorStore not found");

        metric_function = Metric::get_metric_function(metric_type);
    }

    bool get_tensor(ObjectId oid, float* res)
    {
        VTensor* vtensor;
        if (!tensor_store->get(oid, &vtensor)) {
            return false;
        }
        const auto tensor = vtensor->get_tensor();
        std::memcpy(res, tensor, sizeof(float) * tensor_store->tensors_dim());
        tensor_store->unpin(*vtensor);
        return true;
    }

    ObjectId eval(const Binding& binding) override
    {
        const ObjectId lhs_expr_oid = lhs_expr->eval(binding);
        const ObjectId rhs_expr_oid = rhs_expr->eval(binding);

        auto lhs_tensor = std::make_unique<float[]>(tensor_store->tensors_dim());
        auto rhs_tensor = std::make_unique<float[]>(tensor_store->tensors_dim());

        if (!get_tensor(lhs_expr_oid, lhs_tensor.get()) || !get_tensor(rhs_expr_oid, rhs_tensor.get())) {
            return ObjectId::get_null();
        }

        const float distance = metric_function(
            lhs_tensor.get(),
            rhs_tensor.get(),
            tensor_store->tensors_dim()
        );

        return Common::Conversions::pack_float(distance);
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }
};
} // namespace MQL
