#pragma once

#include <cassert>
#include <memory>
#include <set>

#include "query/parser/op/op_visitor.h"
#include "query/parser/op/sparql/ops.h"
#include "query/parser/expr/sparql_exprs.h"
#include "rewrite_rule.h"

namespace SPARQL {
/**
 * According to (Schmidt, 2010) the following rewrite rule is valid:
 * Filter<R1 AND R2>(OP) = Filter<R1>(Filter<R2>(OP))
 * So, we will express the filter as Filter<R1, R2>(OP)
 */
class DecompressAndFilter : public RewriteRule {
public:
    bool is_possible_to_regroup(std::unique_ptr<Op>& unknown_op) override {
        if (!is_castable_to<OpFilter>(unknown_op)) {
            return false;
        }
        auto op_filter = dynamic_cast<OpFilter*>(unknown_op.get());
        for (auto &expr : op_filter->filters) {
            if (is_castable_to<ExprAnd>(expr)) {
                return true;
            }
        }
        return false;
    }

    std::unique_ptr<Op> regroup(std::unique_ptr<Op> unknown_op) override {
        auto op_filter = cast_to<OpFilter>(std::move(unknown_op));
        std::vector<std::unique_ptr<Expr>> new_filters;
        for (auto &expr : op_filter->filters) {
            if (is_castable_to<ExprAnd>(expr)) {
                auto and_expr = cast_to<ExprAnd>(std::move(expr));
                new_filters.push_back(std::move(and_expr->lhs));
                new_filters.push_back(std::move(and_expr->rhs));
            }
            else {
                new_filters.push_back(std::move(expr));
            }
        }
        op_filter->filters = std::move(new_filters);
        return op_filter;
    }
};
} // namespace SPARQL
