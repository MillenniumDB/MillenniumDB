#include "leapfrog_join.h"

#include <cassert>

#include "macros/likely.h"
#include "query/exceptions.h"

using std::vector;

void LeapfrogJoin::_begin(Binding& _parent_binding) {
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
            iters_for_var.push_back(std::move(iter_list));
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

    if (open_terms) {
        down();
    }
}


bool LeapfrogJoin::_next() {
    if (MDB_unlikely(*leapfrog_iters[0]->interruption_requested)) {
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


void LeapfrogJoin::_reset() {
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
    uint_fast32_t p = 0;

    auto min = iters_for_var[level][p]->get_key();
    auto max = iters_for_var[level][iters_for_var[level].size() - 1]->get_key();

    while (min != max) { // min = max means all are equal
        assert(max > min);
        if (MDB_unlikely(*leapfrog_iters[0]->interruption_requested)) {
            throw InterruptedException();
        }
        seeks++;
        if (iters_for_var[level][p]->seek(max)) {
            // after the seek, the previous min is the max
            auto new_max = iters_for_var[level][p]->get_key();
            assert(new_max >= max);
            max = new_max;

            // update the min
            p = (p + 1) % iters_for_var[level].size();
            auto new_min = iters_for_var[level][p]->get_key();
            assert(new_min >= min);
            min = new_min;
        } else {
            return false;
        }
    }
    parent_binding->add(var_order[level], ObjectId(min));
    return true;
}


void LeapfrogJoin::accept_visitor(BindingIterVisitor& visitor) {
    visitor.visit(*this);
}


void LeapfrogJoin::assign_nulls() {
    for (uint_fast32_t lvl = 0; lvl < var_order.size(); lvl++) {
        parent_binding->add(var_order[lvl], ObjectId::get_null());
    }
}
