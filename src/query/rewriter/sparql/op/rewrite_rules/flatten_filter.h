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
 * Filter<R1>(Filter<R2>(OP)) = Filter<R1 AND R2>(OP),
 * So, we will express the filter as Filter<R1, R2>(OP)
 */
class FlattenFilter : public RewriteRule {
public:
    bool is_possible_to_regroup(std::unique_ptr<Op>& unknown_op) override {
        if (!is_castable_to<OpFilter>(unknown_op)) {
            return false;
        }
        auto op_filter = dynamic_cast<OpFilter*>(unknown_op.get());
        return is_castable_to<OpFilter>(op_filter->op);
    }

    std::unique_ptr<Op> regroup(std::unique_ptr<Op> unknown_op) override {
        auto op_filter = cast_to<OpFilter>(std::move(unknown_op));
        auto second_op_filter = cast_to<OpFilter>(std::move(op_filter->op));
        op_filter->filters.insert(
            op_filter->filters.end(),
            std::make_move_iterator(second_op_filter->filters.begin()),
            std::make_move_iterator(second_op_filter->filters.end()));
        op_filter->op = std::move(second_op_filter->op);
        return op_filter;
    }
};
} // namespace SPARQL
