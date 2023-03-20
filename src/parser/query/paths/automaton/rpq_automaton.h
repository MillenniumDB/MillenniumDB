#pragma once

#include <functional>
#include <set>
#include <string>
#include <vector>

#include "base/ids/object_id.h"

/*
A path is represented by a regular expression over edge types in a query.
The following classes are used to build an automaton that represent this
regular expression.
*/


// RPQ_DFA: Regular path query deterministic finite automaton
class RPQ_DFA {
public:
    struct Transition {
        uint_fast32_t from;
        uint_fast32_t to;

        // Type of the transition. Epsilon transitions are represented by nullptr
        ObjectId type_id;

        // Inverse represents the direction of the edge:
        // false: from->to
        // true:  to->from
        bool inverse;

        Transition(uint_fast32_t from, uint_fast32_t to, ObjectId type_id, bool inverse) :
            from    (from),
            to      (to),
            type_id (type_id),
            inverse (inverse) { }

        bool operator==(const Transition& other) const {
            return from == other.from && to == other.to && type_id == other.type_id && inverse == other.inverse;
        }
    };

    uint32_t start_state = 0;

    // Number of states. States are numbered from 0 to (total_states-1)
    uint32_t total_states = 1;

    // Number of final states.
    uint32_t total_final_states;

    // Final states before `set_final_state()` is called
    std::vector<bool> is_final_state;

    // from_to_connections[i] is a list of the transitions that start at state i
    std::vector<std::vector<Transition>> from_to_connections;

    // Stores the distance to the nearest end state.
    std::vector<uint32_t> distance_to_final;

    void print();
};




/* TODO: clean and re-comment
RPQ_NFA: Regular path query nondeterministic finite automaton
RPQAutomaton represents a Non-Deterministic Finite Automaton (NFA) with  epsilon
transitions. This class builds an automaton, and transform it into an automaton
without epsilon transitions.
The automaton is built using Thompson Algorithm, each operator allowed by the
language is represented in the corresponding IPath subclass.
There are some methods used  to  transform  the  automaton  into a  final automaton.
The final automaton does not have  epsilon  transitions  and  as  maximum  two final
states. All transformations and the description of this are handled in the following
methods:
 1.  delete_mergeable_states
 2.  delete_epsilon_transitions
 3.  set_final_state
 4.  delete_absorbing_states
States of automaton are not emulated by a specific class. A state is only represented
by a number i, that indicates that the transitions of this state are stored in the i
position of the from_to_connections, to_from_connections and transitions vectors.
Final the distance to a final state is computed, this metric can be used as heuristic
by a path finder algorithm to select the state which is nearest to final state.
*/
class RPQ_NFA {
public:
    struct Transition {
        uint_fast32_t from;
        uint_fast32_t to;

        // Type of the transition. Epsilon transitions are represented by nullptr
        const std::string* type;

        // Inverse represents the direction of the edge:
        // false: from->to
        // true:  to->from
        bool inverse;

        Transition(uint_fast32_t from, uint_fast32_t to, const std::string* type, bool inverse) :
            from    (from),
            to      (to),
            type    (type),
            inverse (inverse) { }

        bool operator==(const Transition& other) const {
            // type might be nullptr so we can't directly compare `*type == *other.type`
            if (from == other.from && to == other.to && inverse == other.inverse) {
                if (type == nullptr) {
                    return other.type == nullptr;
                } else if (other.type != nullptr) {
                    return *type == *other.type;
                }
            }
            return false;
        }
    };

    // Start state
    uint32_t start = 0;

    // Number of states
    uint32_t total_states = 1;

    // `final_state` will be set at the automaton transformation in the method
    // `set_final_state()`, until then there can be multiple final states
    // uint32_t final_state;

    // True if the start state is a final state
    // bool start_is_final = false;

    // Final states before `set_final_state()` is called
    std::set<uint32_t> end_states;

    // outer_transitions[i] is a list of the transitions that start at state i
    std::vector<std::vector<Transition>> outer_transitions;

    // inner_transitions[i] is a list of the transitions that reach the state i
    std::vector<std::vector<Transition>> inner_transitions;

    // Stores the distance to end state. It can be used by
    // AStar algorithm in enum and check binding_id_iter algorithms.
    // std::vector<uint32_t> distance_to_final;

    // ----- Methods to handle automaton transformations -----

    // Check if two states are mergeable and merge them if is possible.
    void delete_mergeable_states();

    // Delete epsilon transitions of the automaton
    void delete_epsilon_transitions();

    // Delete states that can not be reached from start
    void delete_unreachable_states();

    // Delete states that can not reach to any state of end_states set
    void delete_absorbing_states();

    // Return epsilon closure of state state is not included
    std::set<uint32_t> get_epsilon_closure(uint32_t state);

    // Return reachable states from 'source', 'inverse'=false use
    // outer_transitions and 'inverse'=true use to_from_connections
    std::set<uint32_t>get_reachable_states(uint32_t source, bool inverse);

    // merge 'source' into 'destiny'
    void merge_states(uint32_t destiny, uint32_t source);

    // Compute the minimum distance between final_state and a state of the automaton
    void calculate_distance_to_final_state();

    // Sort the transitions for each state according to the distance to final state
    void sort_state_transition(uint32_t state);
    void sort_transitions();

    inline uint32_t get_start() const noexcept { return start; }
    inline uint32_t get_total_states() const noexcept  { return total_states; }

    void print();

    // Add states from other to this, rename 'other' states, update 'other'
    // end states to be consistent with rename. Don't update 'other' connections
    void rename_and_merge(RPQ_NFA& other);

    // Add a transition to automaton
    void add_transition(const Transition& transition);

    // Add a transition (from, to, "", false)
    void add_epsilon_transition(uint32_t from, uint32_t to);

    // Apply transformations to get final automaton
    RPQ_DFA transform_automaton(std::function<ObjectId(const std::string&)>);

    // bool state_partitions_to_automaton(uint32_t state);

    // std::pair<RPQAutomaton, RPQAutomaton> partition_automaton(uint32_t split_state);

    // std::pair<RPQAutomaton, RPQAutomaton> partition_by_transition(uint32_t from, uint32_t to);
};