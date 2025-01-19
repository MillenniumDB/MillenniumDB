#pragma once

#include <memory>
#include <vector>

#include "query/parser/expr/expr.h"
#include "storage/index/tensor_store/metric.h"

namespace MQL {

class ExprTensorDistance : public Expr {
public:
    Metric::MetricType metric_type;

    std::string tensor_store_name;

    std::unique_ptr<Expr> expr;

    std::unique_ptr<Expr> expr_ref;
    std::vector<float> tensor_ref;

    ExprTensorDistance(
        std::string&& tensor_store_name,
        Metric::MetricType metric_type,
        std::unique_ptr<Expr>&& expr,
        std::unique_ptr<Expr>&& expr_ref
    ) :
        metric_type(metric_type),
        tensor_store_name(std::move(tensor_store_name)),
        expr(std::move(expr)),
        expr_ref(std::move(expr_ref))
    { }

    ExprTensorDistance(
        std::string&& tensor_store_name,
        Metric::MetricType metric_type,
        std::unique_ptr<Expr>&& expr,
        std::vector<float>&& tensor_ref
    ) :
        metric_type(metric_type),
        tensor_store_name(std::move(tensor_store_name)),
        expr(std::move(expr)),
        tensor_ref(std::move(tensor_ref))
    { }

    virtual std::unique_ptr<Expr> clone() const override
    {
        auto tensor_store_name_clone = tensor_store_name;

        if (expr_ref != nullptr) {
            return std::make_unique<ExprTensorDistance>(
                std::move(tensor_store_name_clone),
                metric_type,
                expr->clone(),
                expr_ref->clone()
            );
        } else {
            auto tensor_ref_clone = tensor_ref;
            return std::make_unique<ExprTensorDistance>(
                std::move(tensor_store_name_clone),
                metric_type,
                expr->clone(),
                std::move(tensor_ref_clone)
            );
        }
    }

    void accept_visitor(ExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override
    {
        std::set<VarId> res;
        auto expr_vars = expr->get_all_vars();
        res.insert(expr_vars.begin(), expr_vars.end());

        if (expr_ref != nullptr) {
            auto expr_ref_vars = expr_ref->get_all_vars();
            res.insert(expr_ref_vars.begin(), expr_ref_vars.end());
        }
        return res;
    }

    bool has_aggregation() const override
    {
        if (expr->has_aggregation()) {
            return true;
        }

        if (expr_ref != nullptr) {
            if (expr_ref->has_aggregation()) {
                return true;
            }
        }

        return false;
    }
};
} // namespace MQL
