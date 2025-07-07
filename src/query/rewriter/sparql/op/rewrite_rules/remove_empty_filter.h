#pragma once

#include <cassert>
#include <memory>
#include <set>

#include "query/parser/op/sparql/op_visitor.h"
#include "query/parser/op/sparql/ops.h"
#include "query/parser/expr/sparql/exprs.h"
#include "rewrite_rule.h"

namespace SPARQL {
/**
 * If a filter does not have any elements then it is trivially true.
 */
class RemoveEmptyFilter : public RewriteRule {
public:
    bool is_possible_to_regroup(std::unique_ptr<Op>& unknown_op) override {
        if (!is_castable_to<OpFilter>(unknown_op)) {
            return false;
        }
        auto op_filter = dynamic_cast<OpFilter*>(unknown_op.get());
        return op_filter->filters.size() == 0;
    }

    std::unique_ptr<Op> regroup(std::unique_ptr<Op> unknown_op) override {
        auto op_filter = cast_to<OpFilter>(std::move(unknown_op));
        return std::move(op_filter->op);
    }
};
} // namespace SPARQL
