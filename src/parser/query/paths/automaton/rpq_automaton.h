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

// Transition represents a transition in the automaton.
struct Transition {
    uint_fast32_t from;
    uint_fast32_t to;

    // Won't be setted until the end of the automaton transformation
    ObjectId type_id;

    // Type of the transition. Epsilon transitions are represents with an empty string
    std::string type;

    // Inverse represents if the direction of the transition:
    // false: from->to
    // true:  to->from
    bool inverse;

    Transition(uint_fast32_t from, uint_fast32_t to, const std::string& type, bool inverse) :
        from    (from),
        to      (to),
        type    (type),
        inverse (inverse) { }

    Transition(uint_fast32_t from, uint_fast32_t to, std::string&& type, bool inverse) :
        from    (from),
        to      (to),
        type    (std::move(type)),
        inverse (inverse) { }

    bool operator==(Transition other) {
        return from == other.from && to == other.to && type == other.type && inverse == other.inverse;
    }
};


/*
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
 4.  delete_absortion_states

States of automaton are not emulated by a specific class. A state is only represented
by a number i, that indicates that the transitions of this state are stored in the i
position of the from_to_connections, to_from_connections and transitions vectors.

Final the distance to a final state is computed, this metric can be used as heuristic
by a path finder algorithm to select the state which is nearest to final state.
*/
class RPQAutomaton {
public:
    // Start state, always is 0
    uint32_t start = 0;

    // Number of states
    uint32_t total_states = 1;

    // `final_state` will be set at the automaton transformation in the method
    // `set_final_state()`, until then there can be multiple final states
    uint32_t final_state;

    // True if the start state is a final state
    bool start_is_final = false;

    // Final states before `set_final_state()` is called
    std::set<uint32_t> end_states;

    // from_to_connections[i] is a list of the transitions that start at state i
    std::vector<std::vector<Transition>> from_to_connections;

    // to_from_connections[i] is a list of the transitions that reach the state i
    // these transitions will be invalid after transforming the automata
    // prefer using from_to_connections since it's not invalidated
    std::vector<std::vector<Transition>> to_from_connections;

    // Stores the distance to end state. It can be used by
    // AStar algorithm in enum and check binding_id_iter algorithms.
    std::vector<uint32_t> distance_to_final;

    // ----- Methods to handle automaton transformations -----

    // Check if two states are mergeable and merge them if is posible.
    void delete_mergeable_states();

    // Delete epsilon transitions of the automaton
    void delete_epsilon_transitions();

    // Delete states that can not be reached from start
    void delete_unreachable_states();

    // Collapse end states to generate a unique final state
    void set_final_state();

    // Delete states that can not reach to any state of end_states set
    void delete_absortion_states();

    // ----- Auxilary methods -----

    // Return epsilon closure of state state is not included
    std::set<uint32_t> get_epsilon_closure(uint32_t state);

    // Return reachable states from 'source', 'inverse'=false use
    // from_to_connections and 'inverse'=true use to_from_connections
    std::set<uint32_t>get_reachable_states(uint32_t source, bool inverse);

    // Connections that starts or reachs to 'source' will be start or reach to 'destiny'
    void merge_states(uint32_t destiny, uint32_t source);

    // Compute the minimum distance between final_state and a state of the automaton
    void calculate_distance_to_final_state();

    // Sort the transitions for each state according to the distance to final state
    void sort_state_transition(uint32_t state);
    void sort_transitions();

    // Access  and modify attibute methods
    inline uint32_t get_start() const noexcept { return start; }
    inline uint32_t get_total_states() const noexcept  { return total_states; }
    inline uint32_t get_final_state() const noexcept  { return final_state; }

    void print();

    // Add states from other to this, rename 'other' states, update 'other'
    // end states to be consistent with rename. Don't update 'other' connections
    void rename_and_merge(RPQAutomaton& other);

    // Add a transition to automaton
    void add_transition(Transition transition);

    // Add a transition (from, to, "", false)
    void add_epsilon_transition(uint32_t from, uint32_t to);

    // Apply transformations to get final automaton
    void transform_automaton(std::function<ObjectId(const std::string&)>);

    // bool state_partitions_to_automaton(uint32_t state);

    // std::pair<RPQAutomaton, RPQAutomaton> partition_automaton(uint32_t split_state);

    // std::pair<RPQAutomaton, RPQAutomaton> partition_by_transition(uint32_t from, uint32_t to);
};
