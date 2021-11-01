#include "selinger_optimizer.h"

#include <cassert>
#include <iomanip>
#include <iostream>
#include <limits>

#include "relational_model/models/quad_model/query_optimizer/plan/join/index_nested_loop_plan.h"

using namespace std;

struct CombinationEnumerator {
    bool* arr;
    int count;
    size_t size;

    // 0111000000
    CombinationEnumerator(size_t _size, size_t ones_needed) :
        size(_size)
    {
        arr = new bool[size];
        for (size_t i = 0; i < size; ++i) {
            arr[i] = false;
        }
        count = ones_needed * -1;
    }

    ~CombinationEnumerator() {
        delete[](arr);
    }

    void sum_one() {
        for (size_t i = 0; i < size /* redundant condition */; ++i) {
            if (arr[i]) {
                // 1 -> 0
                --count;
                arr[i] = false;
            } else {
                // 0 -> 1
                ++count;
                arr[i] = true;
                break;
            }
        }
    }

    bool* get_next_combination() {
        do {
            sum_one();
        } while (count != 0);
        return arr;
    }
};


SelingerOptimizer::SelingerOptimizer(vector<unique_ptr<Plan>> base_plans,
                                     const std::vector<std::string>& var_names) :
    plans_size (base_plans.size()),
    var_names  (var_names)
{
    assert(plans_size > 0);
    optimal_plans = new unique_ptr<Plan>*[plans_size];

    cout << "\nBase Plans:" << plans_size << "\n";
    for (size_t i = 0; i < plans_size; ++i) {
        auto arr_size = nCr(plans_size, i+1);

        optimal_plans[i] = new unique_ptr<Plan>[arr_size];
        optimal_plans[0][i] = move(base_plans[i]);
        optimal_plans[0][i]->print(std::cout, 0, var_names);
        cout << "\n";
    }
}


SelingerOptimizer::~SelingerOptimizer() {
    for (size_t i = 0; i < plans_size; ++i) {
        delete[](optimal_plans[i]);
    }
    delete[](optimal_plans);
}


unique_ptr<Plan> SelingerOptimizer::get_plan() {
    for (size_t i = 2; i <= plans_size; ++i) {
        auto combination_enumerator = CombinationEnumerator(plans_size, i);

        // for each combination get its optimal sub plan
        auto total_combinations = nCr(plans_size, i);

        for (uint_fast32_t c = 0; c < total_combinations; ++c) {
            unique_ptr<Plan> best_plan = nullptr;
            auto best_cost = std::numeric_limits<double>::infinity();
            auto arr = combination_enumerator.get_next_combination();

            // for each one in array explore the plan forking that bit
            // example for 0011010:
            //             0001010 and 0010000
            //             0010010 and 0001000
            //             0011000 and 0000010

            for (size_t bit_pos = 0; bit_pos < plans_size; ++bit_pos) {
                if (arr[bit_pos]) {
                    arr[bit_pos] = false;

                    auto current_plan = make_unique<IndexNestedLoopPlan>(
                        optimal_plans[i-2][get_index(arr, plans_size)]->duplicate(),
                        optimal_plans[0][bit_pos]->duplicate()
                    );

                    auto current_cost = current_plan->estimate_cost();

                    if (best_plan == nullptr || current_cost < best_cost) {
                        best_cost = current_cost;
                        best_plan = move(current_plan);
                    }

                    arr[bit_pos] = true;
                }
            }
            optimal_plans[i-1][get_index(arr, plans_size)] = move(best_plan);
        }
    }
    return move(optimal_plans[plans_size-1][0]);
}


uint64_t SelingerOptimizer::nCr(uint_fast32_t n, uint_fast32_t r) {
    if (n < r) {
        return 0;
    }

    uint64_t res = 1;
    for (uint_fast32_t i = 0; i < r; ++i) {
        res *= n-i;
        res /= i+1;
    }

    return res;
}


uint_fast32_t SelingerOptimizer::get_index(bool* arr, uint_fast32_t size) {
    uint_fast32_t res = 0;
    uint_fast32_t k   = 0;

    for (uint_fast32_t i = 0; i < size; ++i) {
        if (arr[i] == 1) {
            ++k;
            res += nCr(i, k);
        }
    }
    return res;
}