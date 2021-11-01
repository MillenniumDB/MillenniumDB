#ifndef QUAD_MODEL__GREEDY_OPTIMIZER_H_
#define QUAD_MODEL__GREEDY_OPTIMIZER_H_

#include "relational_model/models/quad_model/query_optimizer/plan/plan.h"

class GreedyOptimizer {
public:
    static std::unique_ptr<Plan> get_plan(std::vector<std::unique_ptr<Plan>> base_plans,
                                          const std::vector<std::string>& var_names);
};

#endif // QUAD_MODEL__GREEDY_OPTIMIZER_H_
