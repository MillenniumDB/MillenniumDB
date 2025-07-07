#pragma once

#include <cassert>
#include <memory>
#include <set>

#include "query/parser/op/sparql/op_visitor.h"
#include "query/parser/op/sparql/ops.h"
#include "rewrite_rule.h"

namespace SPARQL {
/**
 * According to (Schmidt, 2010) the following rewrite rule is valid:
 * Filter(A1 UNION A2) = Filter(A1) UNION Filter (A2)
 */
class DistributeFilterOverUnion : public RewriteRule {
public:
    bool is_possible_to_regroup(std::unique_ptr<Op>& unknown_op) override {
        if (!is_castable_to<OpFilter>(unknown_op)) {
            return false;
        }
        auto op_filter = dynamic_cast<OpFilter*>(unknown_op.get());
        return (is_castable_to<OpUnion>(op_filter->op));
    }

    std::unique_ptr<Op> regroup(std::unique_ptr<Op> unknown_op) override {
        //return unknown_op;
        auto op_filter = cast_to<OpFilter>(std::move(unknown_op));
        auto op_union = cast_to<OpUnion>(std::move(op_filter->op));

        std::vector<std::unique_ptr<Op>> ops;
        for (auto &child : op_union->unions) {
            ops.push_back(std::make_unique<OpFilter>(
                std::move(child),
                clone_expr_vector(op_filter->filters)
            ));
        }
        return std::make_unique<OpUnion>(std::move(ops));
    }

private:
    std::vector<std::unique_ptr<Expr>> clone_expr_vector(std::vector<std::unique_ptr<Expr>> &exprs) {
        std::vector<std::unique_ptr<Expr>> new_exprs;
        for (auto &expr : exprs) {
            new_exprs.push_back(expr->clone());
        }
        return new_exprs;
    }
};
} // namespace SPARQL
