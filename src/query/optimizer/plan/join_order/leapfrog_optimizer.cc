#include "leapfrog_optimizer.h"

#include <map>

#include "query/executor/binding_iter/index_nested_loop_join.h"
#include "query/executor/binding_iter/leapfrog_join.h"
#include "query/executor/binding_iter/scan_ranges/assigned_var.h"
#include "query/optimizer/plan/join_order/greedy_optimizer.h"
#include "query/optimizer/plan/join_order/leapfrog_estimator.h"
#include "query/query_context.h"
#include "storage/index/leapfrog/leapfrog_iter.h"

using namespace std;

map<VarId, vector<Plan*>> get_var2plans(const vector<unique_ptr<Plan>>& base_plans)
{
    map<VarId, vector<Plan*>> res;

    auto binding_size = get_query_ctx().get_var_size();
    for (size_t i = 0; i < binding_size; i++) {
        res.insert(make_pair(VarId(i), vector<Plan*>()));
    }
    for (auto& plan : base_plans) {
        for (auto var : plan->get_vars()) {
            res[var].push_back(plan.get());
        }
    }

    return res;
}


unique_ptr<BindingIter> LeapfrogOptimizer::try_get_iter_without_assigned(
    const vector<unique_ptr<Plan>>& base_plans
)
{
    set<VarId> intersection_vars;
    set<VarId> enumeration_vars;
    vector<VarId> var_order;

    auto var2plans = get_var2plans(base_plans);

    // Construct intersection_vars and enumeration_vars
    for (auto&& [var, plans] : var2plans) {
        switch (plans.size()) {
        case 0:
            break;
        case 1:
            enumeration_vars.insert(var);
            break;
        default: // > 1
            intersection_vars.insert(var);
        }
    }

    // Handle border case: full cross products
    if (intersection_vars.size() == 0) {
        return nullptr;
    }

    // var, cost, is_chosen
    vector<tuple<VarId, double, bool>> var_costs;

    for (auto var : intersection_vars) {
        vector<unique_ptr<LeapfrogIter>> var_lf_iters;

        set<VarId> enum_vars;
        for (auto& plan : base_plans) {
            auto plan_vars = plan->get_vars();
            if (plan_vars.find(var) == plan_vars.end())
                continue;

            for (auto plan_var : plan_vars) {
                if (plan_var != var) {
                    enum_vars.insert(plan_var);
                }
            }
        }

        vector<VarId> tmp_var_order { var };
        for (auto enum_var : enum_vars) {
            tmp_var_order.push_back(enum_var);
        }

        for (auto& plan : base_plans) {
            auto plan_vars = plan->get_vars();
            if (plan_vars.find(var) != plan_vars.end()) {
                uint_fast32_t enumeration_level = 1;
                if (!plan->get_leapfrog_iter(var_lf_iters, tmp_var_order, enumeration_level)) {
                    return nullptr;
                }
            }
        }

        // check not using AssignedVar
        for (auto& lf_iter : var_lf_iters) {
            for (auto& range : lf_iter->initial_ranges) {
                auto ptr = range.get();
                if (dynamic_cast<AssignedVar*>(ptr) != nullptr) {
                    return nullptr;
                }
            }
        }

        LeapfrogEstimator estimator(var, std::move(var_lf_iters));
        auto cost = estimator.estimate_var_cost();
        if (cost < 0) {
            return nullptr;
        }
        var_costs.push_back({ var, cost, false });
        // std::cout << "LeapfrogEstimator for var " << var << ": " << cost << "\n";
    }

    // order vars by cost
    std::sort(var_costs.begin(), var_costs.end(), [](auto& left, auto& right) {
        return std::get<1>(left) < std::get<1>(right);
    });

    var_order.push_back(std::get<0>(var_costs[0]));
    std::get<2>(var_costs[0]) = true;

    set choosen_vars { std::get<0>(var_costs[0]) };

    auto count_fixed = [&](VarId var) {
        int max_fixed = 0;

        for (auto& plan : base_plans) {
            auto plan_vars = plan->get_vars();
            if (plan_vars.find(var) != plan_vars.end()) {
                int open_vars = 0;
                for (auto plan_var : plan_vars) {
                    if (choosen_vars.find(plan_var) == choosen_vars.end()) {
                        open_vars++;
                    }
                }
                int current_fixed = plan->relation_size() - open_vars;
                if (current_fixed > max_fixed)
                    max_fixed = current_fixed;
            }
        }

        return max_fixed;
    };

    auto is_connected_with_fixed = [&](VarId var, int min_fixed) {
        int max_fixed = 0;
        bool is_connected = false;

        for (auto& plan : base_plans) {
            auto plan_vars = plan->get_vars();
            if (plan_vars.find(var) != plan_vars.end()) {
                int open_vars = 0;
                for (auto plan_var : plan_vars) {
                    if (choosen_vars.find(plan_var) == choosen_vars.end()) {
                        open_vars++;
                    } else {
                        is_connected = true;
                    }
                }
                int current_fixed = plan->relation_size() - open_vars;
                if (current_fixed > max_fixed)
                    max_fixed = current_fixed;
            }
        }

        return max_fixed >= min_fixed && is_connected;
    };

    // cost 0 goes first
    for (auto& [var, cost, chosen] : var_costs) {
        if (chosen)
            continue;
        if (cost == 0) {
            chosen = true;
            var_order.push_back(var);
            choosen_vars.insert(var);
        }
    }
    while (true) {
begin_while:
        // prefer count_fixed(var) >= 2 and not cross product
        for (auto& [var, cost, chosen] : var_costs) {
            if (chosen)
                continue;

            if (is_connected_with_fixed(var, 2)) {
                chosen = true;
                var_order.push_back(var);
                choosen_vars.insert(var);
                goto begin_while;
            }
        }

        // then prefer count_fixed(var) >= 2 and allow cross product
        for (auto& [var, cost, chosen] : var_costs) {
            if (chosen)
                continue;

            if (count_fixed(var) >= 2) {
                chosen = true;
                var_order.push_back(var);
                choosen_vars.insert(var);
                goto begin_while;
            }
        }

        for (auto& [var, cost, chosen] : var_costs) {
            if (chosen)
                continue;

            if (count_fixed(var) >= 1) {
                chosen = true;
                var_order.push_back(var);
                choosen_vars.insert(var);
                goto begin_while;
            }
        }
        for (auto& [var, cost, chosen] : var_costs) {
            if (chosen)
                continue;

            chosen = true;
            var_order.push_back(var);
            choosen_vars.insert(var);
            goto begin_while;
        }
        break;
    }

    uint_fast32_t enumeration_level = var_order.size();

    for (auto enumeration_var : enumeration_vars) {
        var_order.push_back(enumeration_var);
    }

    // cout << "Var order: [";
    // for (const auto& var : var_order) {
    //     cout << " " << var;
    // }
    // cout << " ]\n";

    // second pass on the base_plans, now creating the leapfrog iterators using the variable order constructed
    vector<unique_ptr<LeapfrogIter>> leapfrog_iters;

    std::vector<std::unique_ptr<Plan>> lf_plans;
    std::vector<std::unique_ptr<Plan>> remaining_plans;

    for (const auto& plan : base_plans) {
        if (plan->get_leapfrog_iter(leapfrog_iters, var_order, enumeration_level)) {
            lf_plans.push_back(plan->clone());
        } else {
            remaining_plans.push_back(plan->clone());
        }
    }

    if (leapfrog_iters.size() < 2) {
        return nullptr;
    }


    std::unique_ptr<BindingIter> lf_iter =
        make_unique<LeapfrogJoin>(std::move(leapfrog_iters), std::move(var_order), enumeration_level);

    if (remaining_plans.empty()) {
        return lf_iter;
    }

    // check if any intersection var transformed to enumeration
    auto var2lf_plans = get_var2plans(lf_plans);
    set<VarId> lf_enumeration_vars;

    // Construct intersection_vars and enumeration_vars
    for (auto&& [var, plans] : var2lf_plans) {
        if (plans.size() > 1) {
            lf_enumeration_vars.insert(var);
        }
    }
    if (enumeration_vars != lf_enumeration_vars) {
        return nullptr;
    }

    double best_remaining_cost = std::numeric_limits<double>::infinity();
    for (size_t i = 0; i < remaining_plans.size(); i++) {
        auto current_element_cost = remaining_plans[i]->estimate_cost();
        if (current_element_cost < best_remaining_cost) {
            best_remaining_cost = current_element_cost;
        }
    }

    auto lf_iter_cost = std::get<1>(var_costs[0]);
    if (lf_iter_cost < best_remaining_cost) {
        for (size_t i = 0; i < remaining_plans.size(); i++) {
            remaining_plans[i]->set_input_vars(intersection_vars);
        }
        auto remaining_plan = GreedyOptimizer::get_plan(remaining_plans);
        return std::make_unique<IndexNestedLoopJoin>(
            std::move(lf_iter),
            remaining_plan->get_binding_iter()
        );
    }

    return nullptr;
}


unique_ptr<BindingIter> LeapfrogOptimizer::try_get_iter_with_assigned(
    const vector<unique_ptr<Plan>>& base_plans
)
{
    map<VarId, pair<double, size_t>> var2cost;
    set<VarId> unchosen_intersection_vars;
    set<VarId> unchosen_connected_intersection_vars;
    set<VarId> enumeration_vars;
    vector<VarId> var_order;

    auto var2plans = get_var2plans(base_plans);

    // Construct var2cost, unchosen_intersection_vars and enumeration_vars
    for (auto&& [var, plans] : var2plans) {
        double best_cost = std::numeric_limits<double>::infinity();
        for (auto plan : plans) {
            auto cost = plan->estimate_cost();
            if (cost < best_cost) {
                best_cost = cost;
            }
        }

        switch (plans.size()) {
        case 0:
            break;
        case 1:
            enumeration_vars.insert(var);
            break;
        default: // > 1
            unchosen_intersection_vars.insert(var);
        }
        var2cost.insert(make_pair(var, make_pair(best_cost, plans.size())));
    }

    while (!unchosen_intersection_vars.empty()) {
        double choosen_cost = std::numeric_limits<double>::infinity();
        size_t choosen_appearances = 0;
        uint_fast32_t choosen_var_id = UINT_FAST32_MAX;

        if (!unchosen_connected_intersection_vars.empty()) {
            for (auto var : unchosen_connected_intersection_vars) {
                auto [current_cost, current_appearances] = var2cost[var];
                if (current_cost < choosen_cost
                    || (current_cost == choosen_cost && current_appearances > choosen_appearances))
                {
                    choosen_var_id = var.id;
                    choosen_cost = current_cost;
                    choosen_appearances = current_appearances;
                }
            }
        } else {
            for (auto var : unchosen_intersection_vars) {
                auto [current_cost, current_appearances] = var2cost[var];
                if (current_cost < choosen_cost
                    || (current_cost == choosen_cost && current_appearances > choosen_appearances))
                {
                    choosen_var_id = var.id;
                    choosen_cost = current_cost;
                    choosen_appearances = current_appearances;
                }
            }
        }
        VarId choosen_var(choosen_var_id);
        unchosen_intersection_vars.erase(choosen_var);
        unchosen_connected_intersection_vars.erase(choosen_var);

        // update unchosen_connected_intersection_vars
        for (auto plan : var2plans[choosen_var]) {
            for (auto var : plan->get_vars()) {
                if (unchosen_intersection_vars.find(var) != unchosen_intersection_vars.end()) {
                    unchosen_connected_intersection_vars.insert(var);
                }
            }
        }
        var_order.push_back(choosen_var);
    }

    uint_fast32_t enumeration_level = var_order.size();

    for (auto enumeration_var : enumeration_vars) {
        var_order.push_back(enumeration_var);
    }

    // cout << "Var order: [";
    // for (const auto& var : var_order) {
    //     cout << " " << var;
    // }
    // cout << " ]\n";

    // second pass on the base_plans, now creating the leapfrog iterators using the variable order constructed
    vector<unique_ptr<LeapfrogIter>> leapfrog_iters;
    for (const auto& plan : base_plans) {
        if (!plan->get_leapfrog_iter(leapfrog_iters, var_order, enumeration_level)) {
            return nullptr;
        }
    }

    return make_unique<LeapfrogJoin>(std::move(leapfrog_iters), std::move(var_order), enumeration_level);
}
