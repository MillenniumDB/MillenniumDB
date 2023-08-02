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
  * OP1 Filter (Not Exists OP2) = OP1 MINUS OP2
  * if there is a safe var in OP2 that is in OP1.
 */
class NotExistsToMinus : public RewriteRule {
public:
    bool is_possible_to_regroup(std::unique_ptr<Op>& unknown_op) override {
        if (!is_castable_to<OpFilter>(unknown_op)) {
            return false;
        }
        auto op_filter = dynamic_cast<OpFilter*>(unknown_op.get());
        auto op1_safe_vars = op_filter->op->get_safe_vars();
        for (auto& filter: op_filter->filters) {
            if (is_castable_to<ExprNotExists>(filter)) {
                return true;
            }
        }
        return false;
    }

    std::unique_ptr<Op> regroup(std::unique_ptr<Op> unknown_op) override {
        auto op_filter = cast_to<OpFilter>(std::move(unknown_op));
        auto resulting_op = std::move(op_filter->op);
        std::vector<std::unique_ptr<Expr>> leftover_filters;
        for (auto& filter: op_filter->filters) {
            if (is_castable_to<ExprNotExists>(filter)) {
                auto not_exists_expr = cast_to<ExprNotExists>(std::move(filter));
                resulting_op = std::make_unique<OpNotExists>(
                    std::move(resulting_op),
                    std::move(not_exists_expr->op)
                );
            }
            else {
                leftover_filters.push_back(std::move(filter));
            }
        }
        if (leftover_filters.size() > 0) {
            return std::make_unique<OpFilter>(
                std::move(resulting_op),
                std::move(leftover_filters)
            );
        }
        return resulting_op;
    }
};
} // namespace SPARQL
