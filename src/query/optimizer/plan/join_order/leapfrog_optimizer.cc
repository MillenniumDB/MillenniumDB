#include "leapfrog_optimizer.h"

#include <map>
#include <iostream>

#include "storage/index/leapfrog/leapfrog_iter.h"
#include "query/executor/binding_iter/leapfrog_join.h"

using namespace std;

unique_ptr<BindingIter> LeapfrogOptimizer::try_get_iter(
    const vector<unique_ptr<Plan>>& base_plans,
    const size_t binding_size)
{
    map<VarId, vector<Plan*>> var2plans;
    map<VarId, pair<double, std::size_t>> var2cost;
    set<VarId> unchosen_intersection_vars;
    set<VarId> unchosen_connected_intersection_vars;
    set<VarId> enumeration_vars;
    vector<VarId> var_order;

    // Construct var2plans
    for (std::size_t i = 0; i < binding_size; i++) {
        var2plans.insert( make_pair(VarId(i), std::vector<Plan*>()) );
    }
    for (auto& plan : base_plans) {
        for (auto var : plan->get_vars()) {
            var2plans[var].push_back(plan.get());
        }
    }

    // Construct var2cost, unchosen_intersection_vars and enumeration_vars
    for (auto&&[var, plans] : var2plans) {
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
        var2cost.insert( make_pair(var, make_pair(best_cost, plans.size()) ) );
    }

    while (!unchosen_intersection_vars.empty()) {
        double choosen_cost = std::numeric_limits<double>::infinity();
        std::size_t choosen_appearances = 0;
        uint_fast32_t choosen_var_id = UINT_FAST32_MAX;

        if (!unchosen_connected_intersection_vars.empty()) {
            for (auto var : unchosen_connected_intersection_vars) {
                auto [current_cost, current_appearances] = var2cost[var];
                if (current_cost < choosen_cost
                    || (current_cost == choosen_cost && current_appearances > choosen_appearances) )
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
                    || (current_cost == choosen_cost && current_appearances > choosen_appearances) )
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

    // TODO: This print should be disabled by default, but being able to print when we want to explain a query.
    // cout << "Var order: [";
    // for (const auto& var : var_order) {
    //     cout << " " << var_names[var.id] << "(" << var.id << ")";
    // }
    // cout << " ]\n";

    // second pass on the base_plans, now creating the leapfrog iterators using the variable order constructed
    vector<unique_ptr<LeapfrogIter>> leapfrog_iters;
    for (const auto& plan : base_plans) {
        if (!plan->get_leapfrog_iter(leapfrog_iters, var_order, enumeration_level)) {
            return nullptr;
        }
    }

    return make_unique<LeapfrogJoin>(
        std::move(leapfrog_iters),
        std::move(var_order),
        enumeration_level
    );
}