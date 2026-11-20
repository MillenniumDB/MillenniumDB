#pragma once

#include <optional>

#include "query/optimizer/plan/plan.h"

class GreedyOptimizer {
public:
    // first: the base plan to start from, instead of the one estimated cheapest
    static std::unique_ptr<Plan> get_plan(
        const std::vector<std::unique_ptr<Plan>>& base_plans,
        std::optional<size_t> first = std::nullopt
    );
};
