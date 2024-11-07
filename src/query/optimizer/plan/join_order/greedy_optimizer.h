#pragma once

#include "query/optimizer/plan/plan.h"

class GreedyOptimizer {
public:
    static std::unique_ptr<Plan> get_plan(const std::vector<std::unique_ptr<Plan>>& base_plans);
};
