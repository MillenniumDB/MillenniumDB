#pragma once

#include <cassert>
#include <memory>
#include <set>

#include "query/parser/op/op_visitor.h"
#include "query/parser/op/sparql/ops.h"
#include "rewrite_rule.h"

namespace SPARQL {
/**
 * According to (Schmidt, 2010) the following rewrite rules
 * do not change the semantics of the queries:
 *             ##################
 *             # Distributivity #
 *             ##################
 * (A1 U A2) JOIN AJ = (A1 JOIN AJ) U (A2 JOIN AJ)
 * AJ JOIN (A1 U A2) = (AJ JOIN A1) U (AJ JOIN A2)
 *                   = (A1 JOIN AJ) U (A2 JOIN Aj) Commutativity
 *
 * We will use these rules to extract all unions the most
 * we can, in an effort to create a normal form.
 * We will extend this to the general form so we rewrite the rules
 * as follows:
 * (U_{i=0}^n Ai) JOIN AJ = AJ JOIN (U_{i=0}^n Ai) = U_{i=0}^n(Ai JOIN AJ)
 */
class DistributeJoinUnion : public RewriteRule {
private:
    bool distribute_join_right_to_left = false;
    bool distribute_join_left_to_right = false;

public:
    bool is_possible_to_regroup(std::unique_ptr<Op>& unknown_op) override {
        distribute_join_left_to_right = distribute_join_right_to_left = false;
        if (!is_castable_to<OpJoin>(unknown_op)) {
            return false;
        }
        auto op_join = dynamic_cast<OpJoin*>(unknown_op.get());
        if (is_castable_to<OpUnion>(op_join->rhs)) {
            distribute_join_left_to_right = true;
            return true;
        }
        if (is_castable_to<OpUnion>(op_join->lhs)) {
            distribute_join_right_to_left = true;
            return true;
        }
        return false;
    }

    std::unique_ptr<Op> regroup(std::unique_ptr<Op> unknown_op) override {
        auto op_join = cast_to<OpJoin>(std::move(unknown_op));
        std::unique_ptr<Op> a_j;
        std::unique_ptr<OpUnion> op_union;
        if (distribute_join_left_to_right) {
            a_j = std::move(op_join->lhs);
            op_union = cast_to<OpUnion>(std::move(op_join->rhs));
        }
        else if (distribute_join_right_to_left) {
            op_union = cast_to<OpUnion>(std::move(op_join->lhs));
            a_j = std::move(op_join->rhs);
        }
        else {
            assert(false &&
                   "Regroup is only called if it is possible to regroup");
        }

        std::vector<std::unique_ptr<Op>> joins;
        joins.reserve(op_union->unions.size());
        for (auto& child_i : op_union->unions) {
            joins.push_back(
                std::make_unique<OpJoin>(std::move(child_i), a_j->clone())
            );
        }
        return std::make_unique<OpUnion>(std::move(joins));
    }
};
} // namespace SPARQL
