#pragma once

#include <cassert>
#include <memory>
#include <set>

#include "query/parser/op/op_visitor.h"
#include "query/parser/op/sparql/ops.h"
#include "rewrite_rule.h"

namespace SPARQL {
/**
 * According to (Schmidt, 2010) the following rewrite rule
 * does not change the semantics of the query:
 *             ##################
 *             # Distributivity #
 *             ##################
 * (A1 U A2) OPT Ao = (A1 OPT Ao) U (A2 OPT Ao)
 *
 * We will use this rule to extract all unions the most
 * we can, in an effort to create a normal form.
 * We will extend this to the general form so we rewrite the rules
 * as follows:
 * (U_{i=0}^n Ao) OPT Ao = U_{i=0}^n(Ai OPT Ao)
 */
class DistributeOptionalsToUnion : public RewriteRule {
public:
    bool is_possible_to_regroup(std::unique_ptr<Op>& unknown_op) override {
        if (!is_castable_to<OpOptional>(unknown_op)) {
            return false;
        }
        auto op_optional = dynamic_cast<OpOptional*>(unknown_op.get());
        return (is_castable_to<OpUnion>(op_optional->lhs));
    }

    std::unique_ptr<Op> regroup(std::unique_ptr<Op> unknown_op) override {
        auto op_optional = cast_to<OpOptional>(std::move(unknown_op));
        auto op_union    = cast_to<OpUnion>(std::move(op_optional->lhs));

        std::vector<std::unique_ptr<Op>> distributed_optionals;
        distributed_optionals.reserve(op_union->unions.size());
        for (auto& child : op_union->unions) {
            distributed_optionals.push_back(
                std::make_unique<OpOptional>(
                    std::move(child),
                    op_optional->rhs->clone()
                )
            );
        }

        return std::make_unique<OpUnion>(std::move(distributed_optionals));
    }
};
} // namespace SPARQL
