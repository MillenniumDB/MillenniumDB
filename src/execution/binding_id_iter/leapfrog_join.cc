#include "leapfrog_join.h"

#include <cassert>
#include <iostream>

#include "base/exceptions.h"
#include "storage/index/tuple_buffer/tuple_buffer.h"

using namespace std;

LeapfrogJoin::LeapfrogJoin(vector<unique_ptr<LeapfrogIter>> _leapfrog_iters,
                           vector<VarId>                    _var_order,
                           int_fast32_t                     _enumeration_level) :
    leapfrog_iters    (move(_leapfrog_iters)),
    var_order         (move(_var_order)),
    enumeration_level (_enumeration_level),
    level             (-1)
    { }


void LeapfrogJoin::begin(BindingId& _parent_binding) {
    parent_binding = &_parent_binding;

    // initialize iters_for_var
    for (size_t i = 0; i < var_order.size(); i++) {
        vector<LeapfrogIter*> iter_list;
        for (const auto& leapfrog_iter : leapfrog_iters) {
            // add iter to the list if it contains the var
            for (const auto& intersection_var : leapfrog_iter->get_intersection_vars()) {
                if (intersection_var == var_order[i]) {
                    iter_list.push_back(leapfrog_iter.get());
                    break;
                }
            }
        }
        if (iter_list.size() > 0) {
            iters_for_var.push_back(move(iter_list));
        } else {
            break;
        }
    }

    // open terms
    bool open_terms = true;
    for (auto& lf_iter : leapfrog_iters) {
        if (!lf_iter->open_terms(*parent_binding)) {
            open_terms = false;
            break;
        }
    }

    cout << "enumeration_level: " << enumeration_level << "\n";

    if (open_terms) {
        cout << "open_terms successfully\n";
        down();
    } else {
        cout << "open_terms failed\n";
    }
    cout << "level: " << level << "\n";
}


bool LeapfrogJoin::next() {
    // cout << "next called\n";
    // cout << "level: " << level << "\n";
    if (__builtin_expect(!!(*leapfrog_iters[0]->interruption_requested), 0)) {
        throw InterruptedException();
    }

    while (level >= 0) {
        while (level < enumeration_level) {
            // We try to bind the variable for the current level
            if (find_intersection_for_current_level()) {
                down();
            } else {
                // We are in a previous intersection, so we need to move the last iterator forward
                // to avoid having the same intersection
                while (true) {
                    if (level == 0) {
                        level = -1; // assign level to -1 so the upcoming calls returns false
                        return false;
                    } else {
                        up();
                        if (iters_for_var[level][iters_for_var[level].size() - 1]->next()) {
                            break;
                        }
                    }
                }
            }
        }
        assert(level == enumeration_level);

        for (size_t i = 0; i < leapfrog_iters.size(); i++) {
            if (leapfrog_iters[i]->next_enumeration(*parent_binding)) {
                while (i > 0) {
                    i--;
                    leapfrog_iters[i]->reset_enumeration();
                    leapfrog_iters[i]->next_enumeration(*parent_binding);
                }
                results_found++;
                return true;
            }
        }

        // when enumeration is over backtrack to previous level (not through up())
        level--;

        if (iters_for_var.size() == 0) {
            level = -1; // assign level to -1 so the upcoming calls returns false
            return false;
        } else {
            // We are in a previous intersection, so we need to move the last iterator forward
            // to avoid having the same intersection
            while (level >= 0 && !iters_for_var[level][iters_for_var[level].size() -  1]->next()) {
                up();
            }
        }
    }
    return false;
}


void LeapfrogJoin::reset() {
    bool open_terms = true;
    for (auto& lf_iter : leapfrog_iters) {
        if (!lf_iter->open_terms(*parent_binding)) {
            open_terms = false;
            break;
        }
    }
    level = -1;
    if (open_terms) {
        down();
    }
}


void LeapfrogJoin::up() {
    assert (level >= 0);
    for (uint_fast32_t i = 0; i < iters_for_var[level].size(); i++) {
        iters_for_var[level][i]->up();
    }
    level--;
}


void LeapfrogJoin::down() {
    assert(level < enumeration_level);
    level++;

    if (level < enumeration_level) {
        for (uint_fast32_t i = 0; i < iters_for_var[level].size(); i++) {
            iters_for_var[level][i]->down();
        }

        // sort the corresponding iterators using insertion sort
        for (int_fast32_t i = 1; i < (int_fast32_t) iters_for_var[level].size(); i++) {
            auto aux = iters_for_var[level][i];
            int_fast32_t j;
            for (j = i - 1; j >= 0 && iters_for_var[level][j]->get_key() > aux->get_key(); j--) {
                iters_for_var[level][j + 1] = iters_for_var[level][j];
            }
            iters_for_var[level][j + 1] = aux;
        }
    } else { // level == enumeration_level
        // prepare for enumeration phase
        leapfrog_iters[0]->begin_enumeration();
        for (uint_fast32_t i = 1; i < leapfrog_iters.size(); i++) {
            leapfrog_iters[i]->begin_enumeration();
            leapfrog_iters[i]->next_enumeration(*parent_binding);
        }
    }
}



bool LeapfrogJoin::find_intersection_for_current_level() {
    // cout << "trying to find intersection at level " << level << "\n";
    uint_fast32_t p = 0;

    auto min = iters_for_var[level][p]->get_key();
    auto max = iters_for_var[level][iters_for_var[level].size() - 1]->get_key();

    // cout << "min: " << min << "\n";
    // cout << "max: " << max << "\n";
    // cout << "min: " << ((min & GraphModel::TYPE_MASK) >> 56) << ", " << (min & GraphModel::VALUE_MASK) << "\n";
    // cout << "max: " << ((max & GraphModel::TYPE_MASK) >> 56) << ", " << (max & GraphModel::VALUE_MASK) << "\n";

    while (min != max) { // min = max means all are equal
        if (__builtin_expect(!!(*leapfrog_iters[0]->interruption_requested), 0)) {
            throw InterruptedException();
        }
        seeks++;
        if (iters_for_var[level][p]->seek(max)) {
            // after the seek, the previous min is the max
            max = iters_for_var[level][p]->get_key();

            // update the min
            p = (p + 1) % iters_for_var[level].size();
            min = iters_for_var[level][p]->get_key();
            // cout << "new min: " << ((min & GraphModel::TYPE_MASK) >> 56) << ", " << (min & GraphModel::VALUE_MASK) << "\n";
            // cout << "new max: " << ((max & GraphModel::TYPE_MASK) >> 56) << ", " << (max & GraphModel::VALUE_MASK) << "\n";

        } else {
            // cout << "new min: " << ((min & GraphModel::TYPE_MASK) >> 56) << ", " << (min & GraphModel::VALUE_MASK) << "\n";
            // cout << "new max: " << ((max & GraphModel::TYPE_MASK) >> 56) << ", " << (max & GraphModel::VALUE_MASK) << "\n";
            // cout << "failed to find intersection at level " << level << "\n";
            return false;
        }
    }
    parent_binding->add(var_order[level], ObjectId(min));
    // cout << "found intersection at level " << level << "\n";
    return true;
}


void LeapfrogJoin::analyze(std::ostream& os, int indent) const {
    os << std::string(indent, ' ');
    os << "LeapfrogJoin(found: " << results_found << ", seeks: " << seeks << ")";
}


void LeapfrogJoin::assign_nulls() {
    for (uint_fast32_t lvl = 0; lvl < var_order.size(); lvl++) {
        parent_binding->add(var_order[lvl], ObjectId::get_null());
    }
}
