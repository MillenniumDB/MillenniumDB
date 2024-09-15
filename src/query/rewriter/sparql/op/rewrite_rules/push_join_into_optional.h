#pragma once

#include <cassert>
#include <memory>
#include <set>

#include "query/var_id.h"
#include "query/parser/op/op_visitor.h"
#include "query/parser/op/sparql/ops.h"
#include "rewrite_rule.h"

namespace SPARQL {
/**
 * According to: (J. Salas and A. Hogan / Semantics and Canonicalisation of SPARQL 1.1)
 * For well designed optional patterns (Semantics and complexity of SPARQL), the following rules holds:
 * Q1 JOIN [Q2 OPT Q3] = [Q1 JOIN Q2] OPT Q3
 * [Q1 OPT Q2] JOIN Q3 = [Q1 JOIN Q3] OPT Q2
 */
class PushJoinIntoOptional : public RewriteRule {
public:
    bool is_possible_to_regroup(std::unique_ptr<Op>& unknown_op) override {
        if (!is_castable_to<OpJoin>(unknown_op))
            return false;
        auto op_join = dynamic_cast<OpJoin*>(unknown_op.get());

        if (is_castable_to<OpBasicGraphPattern>(op_join->lhs) &&
            is_castable_to<OpOptional>(op_join->rhs))
        {
            return true;
        } else if (is_castable_to<OpBasicGraphPattern>(op_join->rhs) &&
                   is_castable_to<OpOptional>(op_join->lhs))
        {
            return true;
        }
        return false;
    }

    std::unique_ptr<Op> regroup(std::unique_ptr<Op> unknown_op) override {
        auto op_join = cast_to<OpJoin>(std::move(unknown_op));

        std::unique_ptr<Op> lhs;
        std::unique_ptr<Op> rhs;

        if (is_castable_to<OpBasicGraphPattern>(op_join->lhs) &&
            is_castable_to<OpOptional>(op_join->rhs))
        {
            // Q1 JOIN [Q2 OPT Q3] = [Q1 JOIN Q2] OPT Q3
            auto q1 = cast_to<OpBasicGraphPattern>(std::move(op_join->lhs));
            auto op_optional = cast_to<OpOptional>(std::move(op_join->rhs));

            auto q2 = std::move(op_optional->lhs);
            auto q3 = std::move(op_optional->rhs);

            assert(q1 != nullptr);
            assert(q2 != nullptr);
            assert(q3 != nullptr);

            return std::make_unique<OpOptional>(
                std::make_unique<OpJoin>(std::move(q1), std::move(q2)),
                std::move(q3)
            );
        } else {
            // [Q1 OPT Q2] JOIN Q3 = [Q1 JOIN Q3] OPT Q2
            auto op_optional = cast_to<OpOptional>(std::move(op_join->lhs));
            auto q3 = cast_to<OpBasicGraphPattern>(std::move(op_join->rhs));

            auto q1 = std::move(op_optional->lhs);
            auto q2 = std::move(op_optional->rhs);

            assert(q1 != nullptr);
            assert(q2 != nullptr);
            assert(q3 != nullptr);

            return std::make_unique<OpOptional>(
                std::make_unique<OpJoin>(std::move(q1), std::move(q3)),
                std::move(q2)
            );
        }
    }
};
} // namespace SPARQL
