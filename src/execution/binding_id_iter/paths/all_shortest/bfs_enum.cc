#include "bfs_enum.h"

#include <cassert>

#include "base/ids/var_id.h"
#include "execution/binding_id_iter/paths/path_manager.h"
#include "query_optimizer/quad_model/quad_model.h"
#include "storage/index/bplus_tree/bplus_tree.h"
#include "storage/index/bplus_tree/bplus_tree_leaf.h"
#include "storage/index/record.h"

using namespace std;
using namespace Paths::AllShortest;

BFSEnum::BFSEnum(ThreadInfo*  thread_info,
                 VarId        path_var,
                 Id           start,
                 VarId        end,
                 RPQAutomaton automaton) :
    thread_info (thread_info),
    path_var    (path_var),
    start       (start),
    end         (end),
    automaton   (automaton) { }


void BFSEnum::begin(BindingId& _parent_binding) {
    parent_binding = &_parent_binding;
    first_next     = true;

    iter = nullptr;
    // Add start object id to open and visited
    ObjectId start_object_id(std::holds_alternative<ObjectId>(start) ? std::get<ObjectId>(start)
                                                                     : (*parent_binding)[std::get<VarId>(start)]);

    auto state_inserted = visited.emplace(start_object_id, automaton.get_start(), 0);

    open.push(state_inserted.first.operator->());

    min_ids[2] = 0;
    max_ids[2] = 0xFFFFFFFFFFFFFFFF;
    min_ids[3] = 0;
    max_ids[3] = 0xFFFFFFFFFFFFFFFF;
}


bool BFSEnum::next() {
    // Check if first node is final
    if (first_next) {
        first_next = false;

        auto current_state = open.front();
        auto node_iter     = quad_model.nodes->get_range(&thread_info->interruption_requested,
                                                         Record<1>({ current_state->node_id.id }),
                                                         Record<1>({ current_state->node_id.id }));
        if (node_iter->next() == nullptr) {
            // return false if node does not exists in bd
            open.pop();
            return false;
        }

        if (automaton.start_is_final) {
            auto new_state = visited.emplace(current_state->node_id, automaton.get_final_state(), 0);
            auto path_id = path_manager.set_path(new_state.first.operator->(), path_var);
            parent_binding->add(path_var, path_id);
            parent_binding->add(end, current_state->node_id);

            results_found++;
            return true;
        }
    }
    // check for next enumeration of state_reached
    if (saved_state_reached != nullptr) {
enumeration:
        if (saved_state_reached->path_iter.next()) {
            auto path_id = path_manager.set_path(saved_state_reached, path_var);
            parent_binding->add(path_var, path_id);
            parent_binding->add(end, saved_state_reached->node_id);

            results_found++;
            return true;
        } else {
            saved_state_reached = nullptr;
        }
    }

    while (open.size() > 0) {
        auto current_state = open.front();
        auto state_reached = current_state_has_next(current_state);

        // Check if a new state need to be added to open.
        if (state_reached != visited.end()) {
            open.push(state_reached.operator->());

            if (state_reached->automaton_state == automaton.get_final_state()) {
                saved_state_reached = state_reached.operator->();
                saved_state_reached->path_iter.start_enumeration();
                goto enumeration;
            }
        } else if (saved_state_reached != nullptr) {
            saved_state_reached->path_iter.start_enumeration();
            goto enumeration;
        } else {
            // Pop and visit next state
            iter = nullptr;
            open.pop();
        }
    }
    return false;
}


robin_hood::unordered_node_set<SearchState>::iterator
  BFSEnum::current_state_has_next(const SearchState* current_state)
{
    // check if this is the first time that current_state is explored
    if (iter == nullptr) {
        current_transition = 0;
        // check if automaton state has outer transitions
        if (automaton.from_to_connections[current_state->automaton_state].size() == 0) {
            return visited.end();
        }
        set_iter(current_state);
    }

    // iterate over the remaining transtions of current_state.
    // don't start from the beginning, resume where it left thanks to current_transition and iter
    while (current_transition < automaton.from_to_connections[current_state->automaton_state].size()) {
        auto& transition = automaton.from_to_connections[current_state->automaton_state][current_transition];

        // iterate over records until there is no more records or reach a new state that has not
        // been visited yet
        for (auto child_record = iter->next(); child_record != nullptr; child_record = iter->next()) {
            SearchState next_state(ObjectId(child_record->ids[2]),
                                   transition.to,
                                   current_state->distance + 1);

            // check if next state has already been visited
            auto visited_search = visited.find(next_state);
            if (visited_search != visited.end()) {
                // return next_state only if it has an optimal distance?
                if (visited_search->distance == next_state.distance) {
                    visited_search->path_iter.add(current_state, transition.inverse, transition.type_id);
                    if (next_state.automaton_state == automaton.get_final_state()) {
                        // return but prevent a open.pop()
                        saved_state_reached = visited_search.operator->();
                        return visited.end();
                    }
                }
            } else {
                return visited.emplace(ObjectId(child_record->ids[2]),
                                       transition.to,
                                       current_state->distance + 1,
                                       current_state,
                                       transition.inverse,
                                       transition.type_id).first;
            }
        }
        // construct new iter with the next transition (if there exists a next transition)
        current_transition++;
        if (current_transition < automaton.from_to_connections[current_state->automaton_state].size()) {
            set_iter(current_state);
        }
    }
    return visited.end();
}


void BFSEnum::set_iter(const SearchState* current_state) {
    // Gets current transition object from automaton
    const auto& transition = automaton.from_to_connections[current_state->automaton_state][current_transition];
    // Gets iter from correct bpt with transition.inverse
    if (transition.inverse) {
        min_ids[0] = current_state->node_id.id;
        max_ids[0] = current_state->node_id.id;
        min_ids[1] = transition.type_id.id;
        max_ids[1] = transition.type_id.id;
        iter = quad_model.to_type_from_edge->get_range(&thread_info->interruption_requested,
                                                       Record<4>(min_ids),
                                                       Record<4>(max_ids));
    } else {
        min_ids[0] = transition.type_id.id;
        max_ids[0] = transition.type_id.id;
        min_ids[1] = current_state->node_id.id;
        max_ids[1] = current_state->node_id.id;
        iter = quad_model.type_from_to_edge->get_range(&thread_info->interruption_requested,
                                                       Record<4>(min_ids),
                                                       Record<4>(max_ids));
    }
    bpt_searches++;
}


void BFSEnum::reset() {
    // Empty open and visited
    queue<const SearchState*> empty;
    open.swap(empty);
    visited.clear();
    first_next = true;
    iter       = nullptr;

    // Add start object id to open and visited
    ObjectId start_object_id(std::holds_alternative<ObjectId>(start) ? std::get<ObjectId>(start)
                                                                     : (*parent_binding)[std::get<VarId>(start)]);

    auto state_inserted = visited.emplace(start_object_id, automaton.get_start(), 0);

    open.push(state_inserted.first.operator->());
}


void BFSEnum::assign_nulls() {
    parent_binding->add(end, ObjectId::get_null());
}


void BFSEnum::analyze(std::ostream& os, int indent) const {
    os << std::string(indent, ' ');
    os << "Paths::AllShortest::BFSEnum(bpt_searches: " << bpt_searches << ", found: " << results_found << ")";
}
