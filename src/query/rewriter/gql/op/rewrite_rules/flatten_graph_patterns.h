#pragma once

#include "query/parser/op/gql/graph_pattern/op_basic_graph_pattern.h"
#include "query/parser/op/gql/graph_pattern/op_filter.h"
#include "query/parser/op/gql/graph_pattern/op_graph_pattern.h"
#include "query/parser/op/gql/graph_pattern/op_graph_pattern_list.h"
#include "query/parser/op/gql/graph_pattern/op_path_union.h"
#include "rewrite_rule.h"

namespace GQL {
class FlattenGraphPatterns : public RewriteRule {
public:
    bool is_possible_to_regroup(std::unique_ptr<Op>& unknown_op) override
    {
        auto op_graph_pattern_list = dynamic_cast<OpGraphPatternList*>(unknown_op.get());

        if (op_graph_pattern_list == nullptr) {
            return false;
        }

        int flatten_count = 0;

        for (auto& pattern : op_graph_pattern_list->patterns) {
            auto op_graph_pattern = dynamic_cast<OpGraphPattern*>(pattern.get());

            if (!is_possible_to_regroup_op_graph_pattern(op_graph_pattern)) {
                continue;
            }

            flatten_count++;
            if (flatten_count >= 2) {
                return true;
            }
        }
        return false;
    }

    std::unique_ptr<Op> regroup(std::unique_ptr<Op> unknown_op) override
    {
        auto op_graph_pattern_list = dynamic_cast<OpGraphPatternList*>(unknown_op.get());
        std::vector<std::unique_ptr<Op>> new_pattern_list;
        auto new_bgp = std::make_unique<OpBasicGraphPattern>();

        for (auto& pattern : op_graph_pattern_list->patterns) {
            auto op_graph_pattern = dynamic_cast<OpGraphPattern*>(pattern.get());

            if (!is_possible_to_regroup_op_graph_pattern(op_graph_pattern)) {
                new_pattern_list.push_back(std::move(pattern));
                continue;
            }

            auto op_bgp = dynamic_cast<OpBasicGraphPattern*>(op_graph_pattern->op.get());

            if (op_bgp != nullptr) {
                for (auto& bgp_pattern : op_bgp->patterns) {
                    new_bgp->add_pattern(std::move(bgp_pattern));
                }
            } else {
                new_bgp->add_pattern(std::move(op_graph_pattern->op));
            }
        }

        auto new_graph_pattern = std::make_unique<OpGraphPattern>(std::move(new_bgp), PathMode());
        new_pattern_list.push_back(std::move(new_graph_pattern));
        return std::make_unique<OpGraphPatternList>(std::move(new_pattern_list));
    }

    bool is_possible_to_regroup_op_graph_pattern(const OpGraphPattern* op_graph_pattern)
    {
        return op_graph_pattern != nullptr && op_graph_pattern->mode.selector == PathMode::ANY
            && op_graph_pattern->mode.restrictor == PathMode::DEFAULT
            && op_graph_pattern->mode.path_count == 1 && !op_graph_pattern->path_var_id.has_value();
    }
};
} // namespace GQL
