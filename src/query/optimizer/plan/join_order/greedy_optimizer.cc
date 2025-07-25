#include "greedy_optimizer.h"

#include <limits>

#include "query/optimizer/plan/join/index_nested_loop_plan.h"

std::unique_ptr<Plan> GreedyOptimizer::get_plan(const std::vector<std::unique_ptr<Plan>>& real_base_plans)
{
    assert(!real_base_plans.empty());
    const auto base_plans_size = real_base_plans.size();

    // We have to duplicate the base plans to not affect them outside this function
    std::vector<std::unique_ptr<Plan>> base_plans;
    for (auto& plan : real_base_plans) {
        base_plans.push_back(plan->clone());
    }

    // choose the first scan
    int best_index = 0;
    double best_cost = std::numeric_limits<double>::infinity();
    for (size_t j = 0; j < base_plans_size; j++) {
        // base_plans[j]->set_input_vars(input_vars);
        auto current_element_cost = base_plans[j]->estimate_cost();
        // base_plans[j]->print(std::cout, 0);
        // std::cout << "\n";
        if (current_element_cost < best_cost) {
            best_cost = current_element_cost;
            best_index = j;
        }
    }
    auto root_plan = std::move(base_plans[best_index]);

    // choose the next scan and make a Join (left deep plan)
    for (size_t i = 1; i < base_plans_size; i++) {
        best_index = 0;
        best_cost = std::numeric_limits<double>::infinity();
        std::unique_ptr<Plan> best_step_plan = nullptr;

        for (size_t j = 0; j < base_plans_size; j++) {
            if (base_plans[j] != nullptr
                && !base_plans[j]->cartesian_product_needed(*root_plan) )
            {
                auto nested_loop_plan = std::make_unique<IndexNestedLoopPlan>(
                    root_plan->clone(),
                    base_plans[j]->clone()
                );
                auto nested_loop_cost = nested_loop_plan->estimate_cost();

                if (nested_loop_cost < best_cost) {
                    best_cost = nested_loop_cost;
                    best_index = j;
                    best_step_plan = std::move(nested_loop_plan);
                }
                // auto hash_join_plan = make_unique<HashJoinPlan>(
                //     root_plan->clone(),
                //     base_plans[j]->clone()
                // );
                // auto hash_join_cost = hash_join_plan->estimate_cost();

                // if (hash_join_cost < best_cost) {
                //     best_cost = hash_join_cost;
                //     best_index = j;
                //     best_step_plan = move(hash_join_plan);
                // }
            }
        }

        // All elements would form a cross product, iterate again, allowing cross products
        if (best_cost == std::numeric_limits<double>::infinity()) {
            best_index = 0;

            for (size_t j = 0; j < base_plans_size; j++) {
                if (base_plans[j] == nullptr) {
                    continue;
                }
                auto nested_loop_plan = std::make_unique<IndexNestedLoopPlan>(
                    root_plan->clone(),
                    base_plans[j]->clone()
                );
                auto nested_loop_cost = nested_loop_plan->estimate_cost();

                if (nested_loop_cost <= best_cost) {
                    best_cost = nested_loop_cost;
                    best_index = j;
                    best_step_plan = std::move(nested_loop_plan);
                }
            }
        }
        base_plans[best_index] = nullptr;
        root_plan = std::move(best_step_plan);
    }

    return root_plan;
}
