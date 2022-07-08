#pragma once

#include <functional>
#include <set>
#include <string>
#include <vector>

#include "base/ids/object_id.h"
#include "base/query/query_element.h"

/*
A path is represented by a regular expression over edge types in a query (RPQ).
The following classes are used to build an automaton that represents this
regular expression. This automaton also supports RDPQs (Data RPQs), where each node or edge
can evaluate it's properties (key: value) as a part of the path.
// TODO: explain automaton structure (data transitions only have data checks, edge transitions have
// both data checks (for the edge) and type)
*/

// TODO: Add more operators
// Operators available for property checks
enum class Operators {
    EQ,     // Equal
    NOT_EQ  // Not Equal
};

// Transitions in the automaton (Edge & Data)
class RDPQTransition {
public:
    // States connected by this transition
    uint_fast32_t from;
    uint_fast32_t to;

    // Direction of the transition (only applies to edge transitions)
    // false: from->to
    // true: to->from
    bool inverse;

    // The nature of the transition
    // false: edge transition
    // true: data transition
    bool is_check;

    // Type of the transition for edge transitions
    // if is data transition type will be ""
    std::string type;

    ObjectId type_id;

    // List of property checks for the transition
    std::vector<std::tuple<Operators, std::string, QueryElement>> property_checks;

    // Transition equality
    bool operator==(RDPQTransition other) {
        // Data check transitions
        if (is_check && other.is_check) {
            if (from == other.from && to == other.to && property_checks.size() == other.property_checks.size()) {
                for (size_t i = 0; i < property_checks.size(); i++) {
                    if (property_checks[i] != other.property_checks[i]) {
                        return false;
                    }
                }
                return true;
            }
        }

        // Edge transitions
        if (!is_check && !other.is_check) {
            if (from == other.from && to == other.to && inverse == other.inverse && type == other.type &&
                property_checks.size() == other.property_checks.size())
            {
                for (size_t i = 0; i < property_checks.size(); i++) {
                    if (property_checks[i] != other.property_checks[i]) {
                        return false;
                    }
                }
                return true;
            }
        }

        // Different transitions
        return false;
    }

    // Data transition constructor
    static RDPQTransition make_data_transition(uint_fast32_t from,
                                               uint_fast32_t to,
                                               std::vector<std::tuple<Operators, std::string, QueryElement>>
                                                   property_checks = std::vector<std::tuple<Operators, std::string, QueryElement>>())
    {
        return RDPQTransition(from, to, false, true, "", move(property_checks));
    }

    // Edge transition constructor
    static RDPQTransition make_edge_transition(uint_fast32_t from,
                                               uint_fast32_t to,
                                               bool inverse,
                                               const std::string& type,
                                               std::vector<std::tuple<Operators, std::string, QueryElement>>
                                                   property_checks = std::vector<std::tuple<Operators, std::string, QueryElement>>())
    {
        return RDPQTransition(from, to, inverse, false, type, move(property_checks));
    }

private:
    // General constructor
    RDPQTransition(uint_fast32_t from,
                   uint_fast32_t to,
                   bool inverse,
                   bool is_check,
                   const std::string& type,
                   std::vector<std::tuple<Operators, std::string, QueryElement>> property_checks) :
        from     (from),
        to       (to),
        inverse  (inverse),
        is_check (is_check),
        type     (type),
        property_checks (std::move(property_checks)) { }
};


/*
RDPQAutomaton represents a special Non-Deterministic Finite Automaton (NFA).
This class builds the automaton and transforms it into an optimal one.

This automaton will be referred to as a DE automaton (Data-Edge automaton), where each state can be of
type D (only has data check 'out' transitions) or type E (only has edge 'out' transitions). After a data check transition there
will always be an E-State. After an edge transition there will always be a D-State.

The following rules will allow the DE properties to be maintained properly (as well as the shortest paths property with BFS):

- There is only one START state, of type D, and it can't be an END state.
- The START state does NOT have any 'in' transitions (Ex: x -> START).
- All END states are of type E.
- An END state does NOT have any 'out' transitions (Ex: END -> x).

After the initial construction, the automaton is then transformed into a final automaton by using set_final_state.

The states of the automaton are not emulated by a specific class. A state is only represented
by an integer i, that indicates that the transitions of the current state are stored in the i-th
position of the from_to_connections, to_from_connections and transition vectors.

The distance to the final state results in a metric that can be used as a heuristic
by a path finder algorithm to select the state which is nearest to the final state.
*/
class RDPQAutomaton {
private:
    // Start state, is always 0
    uint32_t start = 0;

    // Final state, will be set at the end of the automaton transformation
    uint32_t final_state;

    // Number of states
    uint32_t total_states = 1;

    // ----- Methods to handle automaton transformations -----

    // Collapse end states to generate a unique final state
    void set_final_state();

    // ----- Auxiliary methods -----

    // Compute the minimum distance between the final_state and a state of the automaton
    void calculate_distance_to_final_state();

    // Sort the transitions for each state according to their distance to the final state
    void sort_state_transition(uint32_t state);
    void sort_transitions();

public:
    // Transitions that start from the i-th state (stored in the i-th position)
    std::vector<std::vector<RDPQTransition>> from_to_connections;

    // Transitions that reach the i-th state (stored in the i-th position)
    // These transitions will be invalid after transforming the automaton (prefer using from_to_connections)
    std::vector<std::vector<RDPQTransition>> to_from_connections;

    // End states before the automaton transformation
    std::set<uint32_t> end_states;

    // Stores the distance to the end state. It can be used by the
    // A* algorithm in enum and check binding_id_iter algorithms.
    std::vector<uint32_t> distance_to_final;

    // Access attribute methods
    inline uint32_t get_start() const noexcept { return start; }
    inline uint32_t get_total_states() const noexcept  { return total_states; }
    inline uint32_t get_final_state() const noexcept  { return final_state; }

    // Print the automaton
    void print();

    // For debugging
    void log(size_t s);

    // Add states from 'other' automaton to this automaton, rename 'other' states, update 'other'
    // end states to be consistent with rename. Don't update 'other' connections.
    void rename_and_merge(RDPQAutomaton& other);

    // Add a transition to the automaton
    void add_transition(RDPQTransition transition);

    // Apply transformations to get the final automaton
    void transform_automaton(std::function<ObjectId(const std::string&)> f);

    // TODO: Refactor to modify the automaton instead of returning a new one
    // Returns the inverse automaton (invalidating this one)
    // RDPQAutomaton invert_automaton();
};
