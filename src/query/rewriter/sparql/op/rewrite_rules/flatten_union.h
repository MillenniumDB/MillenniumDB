#pragma once

#include <cassert>
#include <memory>
#include <set>

#include "query/parser/op/op_visitor.h"
#include "query/parser/op/sparql/ops.h"
#include "rewrite_rule.h"

namespace SPARQL {
/**
 * Flattens unions so that UNION(UNION(A, B), C) = UNION(A,B,C)
 */
class FlattenUnion : public RewriteRule {
public:
    bool is_possible_to_regroup(std::unique_ptr<Op>& unknown_op) override {
        if (!is_castable_to<OpUnion>(unknown_op))
            return false;

        auto op_union = dynamic_cast<OpUnion*>(unknown_op.get());
        for (auto& child : op_union->unions)
            if (is_castable_to<OpUnion>(child))
                return true;
        return false;
    }

    std::unique_ptr<Op> regroup(std::unique_ptr<Op> unknown_op) override {
        auto op_union = cast_to<OpUnion>(std::move(unknown_op));
        // OP_UNION(A1, A2, OP_UNION(A3, OP_UNION(A4, A5)))
        // A1 A2, A3, OP_UNION(A4, A5)
        std::vector<std::unique_ptr<Op>> unions;
        for (auto& child : op_union->unions) {
            if (is_castable_to<OpUnion>(child)) {
                auto child_union = cast_to<OpUnion>(std::move(child));
                for (auto& sub_child : child_union->unions) {
                    unions.push_back(std::move(sub_child));
                }
            }
            else {
                unions.push_back(std::move(child));
            }
        }
        return std::make_unique<OpUnion>(std::move(unions));
    }
};
} // namespace SPARQL
