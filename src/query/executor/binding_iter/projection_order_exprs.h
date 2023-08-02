#pragma once

#include <map>
#include <memory>
#include <utility>
#include <vector>
#include <unordered_map>

#include "query/executor/binding_iter.h"
#include "query/query_context.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"
#include "query/parser/op/sparql/op_select.h"

class ProjectionOrderExpr {
public:
    ProjectionOrderExpr(VarId var_id, std::unique_ptr<BindingExpr>&& expr, bool is_aggregation) :
        var_id(var_id), expr(std::move(expr)), is_aggregation(is_aggregation) { }

    VarId                          var_id;
    std::unique_ptr<BindingExpr> expr;
    bool                           is_aggregation;
};


class ProjectionOrderExprs : public BindingIter {
public:
    ProjectionOrderExprs(
        std::unique_ptr<BindingIter>                             child_id_iter,
        std::vector<ProjectionOrderExpr>                           projection_order_exprs
    ) :
        child_id_iter          (std::move(child_id_iter)),
        projection_order_exprs (std::move(projection_order_exprs)) { }

    void begin(Binding& parent_binding) override;

    void reset() override;

    bool next() override;

    void assign_nulls() override;

    void analyze(std::ostream&, int indent = 0) const override;

private:
    std::unique_ptr<BindingIter>   child_id_iter;
    std::vector<ProjectionOrderExpr> projection_order_exprs;
    Binding*                       parent_binding;
    uint64_t                         count = 0;
};
