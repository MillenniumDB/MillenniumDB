#include "leapfrog_optimizer.h"

#include <map>
#include <iostream>

#include "storage/index/leapfrog/leapfrog_iter.h"
#include "execution/binding_id_iter/leapfrog_join.h"

using namespace std;

unique_ptr<BindingIdIter> LeapfrogOptimizer::try_get_iter(ThreadInfo* thread_info,
                                                          const vector<unique_ptr<Plan>>& base_plans,
                                                          const vector<string>& var_names,
                                                          const size_t binding_size)
{
    map<VarId, vector<Plan*>> var2plans;
    map<VarId, pair<double, std::size_t>> var2cost;
    set<VarId> unchoosen_intersection_vars;
    set<VarId> unchoosen_connected_intersection_vars;
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

    // Construct var2cost, unchoosen_intersection_vars and enumeration_vars
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
                unchoosen_intersection_vars.insert(var);
        }
        var2cost.insert( make_pair(var, make_pair(best_cost, plans.size()) ) );
    }

    while (!unchoosen_intersection_vars.empty()) {
        double choosen_cost = std::numeric_limits<double>::infinity();
        std::size_t choosen_appereances = 0;
        uint_fast32_t choosen_var_id = UINT_FAST32_MAX;

        if (!unchoosen_connected_intersection_vars.empty()) {
            for (auto var : unchoosen_connected_intersection_vars) {
                auto [current_cost, current_appereances] = var2cost[var];
                if (current_cost < choosen_cost
                    || (current_cost == choosen_cost && current_appereances > choosen_appereances) )
                {
                    choosen_var_id = var.id;
                    choosen_cost = current_cost;
                    choosen_appereances = current_appereances;
                }
            }
        } else {
            for (auto var : unchoosen_intersection_vars) {
                auto [current_cost, current_appereances] = var2cost[var];
                if (current_cost < choosen_cost
                    || (current_cost == choosen_cost && current_appereances > choosen_appereances) )
                {
                    choosen_var_id = var.id;
                    choosen_cost = current_cost;
                    choosen_appereances = current_appereances;
                }
            }
        }
        VarId choosen_var(choosen_var_id);
        unchoosen_intersection_vars.erase(choosen_var);
        unchoosen_connected_intersection_vars.erase(choosen_var);

        // update unchoosen_connected_intersection_vars
        for (auto plan : var2plans[choosen_var]) {
            for (auto var : plan->get_vars()) {
                if (unchoosen_intersection_vars.find(var) != unchoosen_intersection_vars.end()) {
                    unchoosen_connected_intersection_vars.insert(var);
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
    cout << "Var order: [";
    for (const auto& var : var_order) {
        cout << " " << var_names[var.id] << "(" << var.id << ")";
    }
    cout << " ]\n";

    // second pass on the base_plans, now creating the leapfrog iterators using the variable order constructed
    vector<unique_ptr<LeapfrogIter>> leapfrog_iters;
    for (const auto& plan : base_plans) {
        if (!plan->get_leapfrog_iter(thread_info, leapfrog_iters, var_order, enumeration_level)) {
            return nullptr;
        }
    }

    return make_unique<LeapfrogJoin>(move(leapfrog_iters), move(var_order), enumeration_level);
}