#include "bfs_smt_enum.h"

#include <cassert>

#include "execution/binding_id_iter/paths/path_manager.h"
#include "query_optimizer/quad_model/quad_model.h"

using namespace std;
using namespace Paths::AnyShortest;

SMTEnum::SMTEnum(ThreadInfo*  thread_info,
                 VarId        path_var,
                 Id           start,
                 VarId        end,
                 SMTAutomaton automaton) :
    thread_info (thread_info),
    path_var    (path_var),
    start       (start),
    end         (end),
    automaton   (automaton) { }


bool SMTEnum::eval_formula(const SMTTransition& transition, ObjectId obj_id) const {
    // TODO:
    // for (const auto& formula : transition.formulas) {
    //     array<uint64_t, 3> min_range_property;
    //     array<uint64_t, 3> max_range_property;

    //     min_range_property[0] = obj_id.id;
    //     max_range_property[0] = obj_id.id;

    //     // TODO: your formula needs to provide the ObjectId of the property key
    //     ObjectId key_id = formula.something;
    //     min_range_property[1] = key_id.id;
    //     max_range_property[1] = key_id.id;

    //     min_range_property[2] = 0;
    //     max_range_property[2] = UINT64_MAX;

    //     auto property_iter = quad_model.object_key_value->get_range(
    //         &thread_info->interruption_requested,
    //         Record<3>(min_range_property),
    //         Record<3>(max_range_property));

    //     auto property_record = property_iter->next();

    //     if (property_record != nullptr) {
    //         ObjectId value_id(property_record->ids[2]);
    //         // TODO: use or save value_id to eval formula
    //     } else {
    //         // TODO: what to do if data is not defined?
    //     }
    // }
    return true;
}


void SMTEnum::begin(BindingId& _parent_binding) {
    parent_binding = &_parent_binding;
    first_next = true;

    iter = nullptr;
    // Add start object id to open and visited
    ObjectId start_object_id(std::holds_alternative<ObjectId>(start) ?
        std::get<ObjectId>(start) :
        (*parent_binding)[std::get<VarId>(start)]);

    auto state_inserted = visited.emplace(automaton.get_start(),
                                          start_object_id,
                                          nullptr,
                                          true,
                                          ObjectId::get_null());

    open.push(state_inserted.first.operator->());

    min_ids[2] = 0;
    max_ids[2] = 0xFFFFFFFFFFFFFFFF;
    min_ids[3] = 0;
    max_ids[3] = 0xFFFFFFFFFFFFFFFF;
}


bool SMTEnum::next() {
    // Check if first node is final
    if (first_next) {
        first_next = false;

        const auto current_state = open.front();
        auto node_iter = quad_model.nodes->get_range(&thread_info->interruption_requested,
                                                     Record<1>({current_state->node_id.id}),
                                                     Record<1>({current_state->node_id.id}));
        // Return false if node does not exists in bd
        if (node_iter->next() == nullptr) {
            open.pop();
            return false;
        }

        if (automaton.start_is_final) {
            auto reached_key = SearchStateSMT(automaton.get_final_state(),
                                              current_state->node_id,
                                              nullptr,
                                              true,
                                              ObjectId::get_null()
                                              //TODO pass empty list of formulas
                                              );

            auto path_id = path_manager.set_path(visited.insert(reached_key).first.operator->(), path_var);
            parent_binding->add(path_var, path_id);
            parent_binding->add(end, current_state->node_id);
            results_found++;
            return true;
        }
    }
    while (open.size() > 0) {
        const auto current_state = open.front();
        auto state_reached = current_state_has_next(current_state);
        // If has next state then state_reached does not point to visited.end()
        if (state_reached != visited.end()) {
            open.push(state_reached.operator->());

            if (state_reached->automaton_state == automaton.get_final_state()) {
                // set binding
                auto path_id = path_manager.set_path(state_reached.operator->(), path_var);
                parent_binding->add(path_var, path_id);
                parent_binding->add(end, state_reached->node_id);
                results_found++;
                return true;
            }
        } else {
            // Pop and visit next state
            iter = nullptr;
            open.pop();
        }
    }
    return false;
}


robin_hood::unordered_node_set<SearchStateSMT>::iterator
    SMTEnum::current_state_has_next(const SearchStateSMT* current_state)
{
    if (iter == nullptr) { // if is first time that State is explored
        current_transition = 0;
        // Check automaton state has transitions
        if (current_transition >= automaton.from_to_connections[current_state->automaton_state].size()) {
            return visited.end();
        }
        // Constructs iter for this state
        set_iter(current_state);
    }

    // Iterate over automaton_start state transtions
    while (current_transition < automaton.from_to_connections[current_state->automaton_state].size()) {
        auto& transition = automaton.from_to_connections[current_state->automaton_state][current_transition];
        auto child_record = iter->next();
        // Iterate over next_childs
        while (child_record != nullptr) {
            // TODO: eval transition.formulas
            // TODO: I imagine you'll need to extract properties from the reached node
            //       I'm not sure what kind of operations you want to support, we don't have
            //       too much implemented, just the usual comparisons (!=, ==, <, <=, >, >=),
            //       When using internal IDs only 2 operations are consistent (== and !=), to
            //       use (<, <= >=, >) the internal ID needs to be converted into a GraphObject.
            //       For the first version I'd focus only on (== and !=) to keep the code simpler
            //       The internal ID of the edge traversed is `ObjectId(child_record->ids[3])`
            //       The internal ID of the reached node is `ObjectId(child_record->ids[2])`
            //       I'll assume you want to extract data only from the reached node
            if (eval_formula(transition, ObjectId(child_record->ids[2]))) {
                // TODO: add formulas into the state
                auto next_state_key = SearchStateSMT(
                    transition.to,
                    ObjectId(child_record->ids[2]),
                    current_state,
                    transition.inverse,
                    transition.type_id);
                auto inserted_state = visited.insert(next_state_key);
                // Inserted_state.second = true if state was inserted in visited
                if (inserted_state.second) {
                    // Return pointer to state in visited
                    return inserted_state.first;
                }
            }

            child_record = iter->next();
        }
        // Constructs new iter
        current_transition++;
        if (current_transition < automaton.from_to_connections[current_state->automaton_state].size()) {
            set_iter(current_state);
        }
    }
    return visited.end();
}


void SMTEnum::set_iter(const SearchStateSMT* current_state) {
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


void SMTEnum::reset() {
    // Empty open and visited
    queue<const SearchStateSMT*> empty;
    open.swap(empty);
    visited.clear();
    first_next = true;
    iter = nullptr;

    // Add start object id to open and visited
    ObjectId start_object_id(std::holds_alternative<ObjectId>(start) ?
        std::get<ObjectId>(start) :
        (*parent_binding)[std::get<VarId>(start)]);

    auto state_inserted = visited.emplace(automaton.get_start(),
                                          start_object_id,
                                          nullptr,
                                          true,
                                          ObjectId::get_null());

    open.push(state_inserted.first.operator->());
}


void SMTEnum::assign_nulls() {
    parent_binding->add(end, ObjectId::get_null());
}


void SMTEnum::analyze(std::ostream& os, int indent) const {
    os << std::string(indent, ' ');
    os << "Paths::AnyShortest::SMTEnum(bpt_searches: " << bpt_searches
       << ", found: " << results_found <<")";
}
