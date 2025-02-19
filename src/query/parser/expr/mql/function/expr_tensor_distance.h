#pragma once

#include <memory>

#include "query/parser/expr/expr.h"
#include "storage/index/tensor_store/metric.h"

namespace MQL {

class ExprTensorDistance : public Expr {
public:
    Metric::MetricType metric_type;

    std::string tensor_store_name;

    std::unique_ptr<Expr> lhs_expr;
    std::unique_ptr<Expr> rhs_expr;

    ExprTensorDistance(
        std::string&& tensor_store_name_,
        Metric::MetricType metric_type_,
        std::unique_ptr<Expr>&& lhs_expr_,
        std::unique_ptr<Expr>&& rhs_expr_
    ) :
        metric_type(metric_type_),
        tensor_store_name(std::move(tensor_store_name_)),
        lhs_expr(std::move(lhs_expr_)),
        rhs_expr(std::move(rhs_expr_))
    { }

    virtual std::unique_ptr<Expr> clone() const override
    {
        auto tensor_store_name_clone = tensor_store_name;
        return std::make_unique<ExprTensorDistance>(
            std::move(tensor_store_name_clone),
            metric_type,
            lhs_expr->clone(),
            rhs_expr->clone()
        );
    }

    void accept_visitor(ExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override
    {
        std::set<VarId> res = lhs_expr->get_all_vars();
        res.merge(rhs_expr->get_all_vars());
        return res;
    }

    bool has_aggregation() const override
    {
        return lhs_expr->has_aggregation() || rhs_expr->has_aggregation();
    }
};
} // namespace MQL
