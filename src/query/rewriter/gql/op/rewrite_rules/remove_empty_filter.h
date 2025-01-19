#pragma once

#include "query/parser/op/gql/graph_pattern/op_filter.h"
#include "rewrite_rule.h"

namespace GQL {
class RemoveEmptyFilter : public RewriteRule {
public:
    RemoveEmptyFilter() { }

    bool is_possible_to_regroup(std::unique_ptr<Op>& unknown_op) override
    {
        auto op_filter = dynamic_cast<OpFilter*>(unknown_op.get());

        if (op_filter == nullptr) {
            return false;
        } else if (op_filter->exprs.empty()) {
            return true;
        }

        return false;
    }

    std::unique_ptr<Op> regroup(std::unique_ptr<Op> unknown_op) override
    {
        auto op_filter = dynamic_cast<OpFilter*>(unknown_op.get());
        return std::move(op_filter->op);
    }
};
} // namespace GQL
