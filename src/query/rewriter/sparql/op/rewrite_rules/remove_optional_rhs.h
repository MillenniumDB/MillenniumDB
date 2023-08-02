#pragma once

#include <cassert>
#include <memory>

#include "query/parser/op/op_visitor.h"
#include "query/parser/op/sparql/ops.h"
#include "rewrite_rule.h"

namespace SPARQL {
/**
  * Recursively removed all rhs of optionals, that is,
  * Op1 OPTIONAL Op2 -> Op1.
  * This is used inside Exists and not exists expressions. As optionals
  * do not increase or decrease the amount of matches.
 */
class RemoveOptionalRhs : public RewriteRule {
public:
    bool is_possible_to_regroup(std::unique_ptr<Op>& unknown_op) override {
        return is_castable_to<OpOptional>(unknown_op);
    }

    std::unique_ptr<Op> regroup(std::unique_ptr<Op> unknown_op) override {
        auto op_optional = cast_to<OpOptional>(std::move(unknown_op));
        return std::move(op_optional->lhs);
    }

};
} // namespace SPARQL
