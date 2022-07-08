#include "rdpq_automaton.h"

#include <iostream>
#include <queue>
#include <stack>
#include <utility>

using namespace std;

// Print the automaton
void RDPQAutomaton::print() {
    for (size_t i = 0; i < from_to_connections.size(); i++) {
        for (auto& t : from_to_connections[i]) {
            string data_check = "";
            for (size_t i = 0; i < t.property_checks.size(); i++) {
                data_check += get<1>(t.property_checks[i]);
                auto op = get<0>(t.property_checks[i]);
                string op_str;
                if (op == Operators::EQ) {
                    op_str = "=";
                } else if (op == Operators::NOT_EQ) {
                    op_str = "!=";
                }
                data_check += op_str;
                data_check += get<2>(t.property_checks[i]).to_string() + ", ";
            }
            if (t.is_check) {
                cout << t.from << "={" << data_check << "}=>" << t.to << "\n";
            } else {
                if (t.property_checks.size() > 0) {
                    cout << t.from << "=[" << (t.inverse ? "^" : "") << t.type << " {" << data_check << "}]=>" << t.to << "\n";
                } else {
                    cout << t.from << "=[" << (t.inverse ? "^" : "") << t.type << "]=>" << t.to << "\n";
                }
            }
        }
    }
    cout << "distance to end: \n";
    for (size_t i = 0; i < distance_to_final.size(); i++) {
        cout << i << ":" << distance_to_final[i] << "\n";
    }
    cout << "end states: { ";
    for (auto& state : end_states) {
        cout << state << "  ";
    }
    cout << "}\n";
    cout << "start state: " << start << "\n";
    cout << "final state: " << final_state << "\n" << endl;
}


// For debugging values
void RDPQAutomaton::log(size_t s) {
    cout << "Log: " << s << endl;
}


// Combine two automatons
void RDPQAutomaton::rename_and_merge(RDPQAutomaton& other) {
    // Add and rename 'other' states to this automaton
    // Renaming consists in the sum of the original states
    // quantity to the id of 'other' states
    auto initial_states = total_states;
    for (size_t i = 0; i < other.from_to_connections.size(); i++) {
        for (auto& t : other.from_to_connections[i]) {
            // Create a transition with renamed states
            if (t.is_check) {  // Data transition
                auto transition = RDPQTransition::make_data_transition(
                    t.from + initial_states,
                    t.to + initial_states,
                    move(t.property_checks));

                // Add transition to this automaton
                add_transition(transition);
            } else {  // Edge transition
                auto transition = RDPQTransition::make_edge_transition(
                    t.from + initial_states,
                    t.to + initial_states,
                    t.inverse,
                    t.type,
                    move(t.property_checks));

                // Add transition to this automaton
                add_transition(transition);
            }
        }
    }

    // Rename 'other' end states
    set<uint32_t> new_end;
    for (auto& end_state : other.end_states) {
        new_end.insert(initial_states + end_state);
    }
    other.end_states = move(new_end);

    // Rename start state for 'other'
    other.start = initial_states;
}


// Add new transition to automaton
void RDPQAutomaton::add_transition(RDPQTransition transition) {
    // Check if connections vector has slots to save from and to
    while (from_to_connections.size() <= transition.from ||
           from_to_connections.size() <= transition.to) {
            vector<RDPQTransition> new_vec;
            from_to_connections.push_back(new_vec);
        }
    while (to_from_connections.size() <= transition.from ||
           to_from_connections.size() <= transition.to) {
            vector<RDPQTransition> new_vec;
            to_from_connections.push_back(new_vec);
        }

    // Check if the new connection already exists
    bool exists = false;
    for (auto& t : from_to_connections[transition.from]) {
        if (transition == t) {
            exists = true;
            break;
        }
    }

    // Add new connection only if it doesn't exist
    if (!exists) {
        from_to_connections[transition.from].push_back(transition);
        to_from_connections[transition.to].push_back(transition);
    }
    // Update number of states
    total_states = from_to_connections.size();
}


// Transform automaton into a final automaton
void RDPQAutomaton::transform_automaton(std::function<ObjectId(const std::string&)> f) {
    // Set the final state
    set_final_state();

    // Sort transitions according to their distance to the final state
    calculate_distance_to_final_state();
    sort_transitions();

    // Set transition ids
    for (size_t i = 0; i < from_to_connections.size(); i++) {
        for (auto& t : from_to_connections[i]) {
            t.type_id = f(t.type);
        }
    }
}

// ----- Transformation handler methods -----


// Collapses all end states into one final state
void RDPQAutomaton::set_final_state() {
    // If there is only one 'end_state' the transformation is not necessary
    if (end_states.size() == 1) {
        // Set final state id as the only state in end_states
        final_state = *end_states.begin();
    } else {
        // If end_states has more than one state, then create an extra state to use as final
        total_states = from_to_connections.size();
        final_state = total_states;
        total_states++;

        // Iterate over current end states & get all states that have transitions towards an end state
        std::set<uint_fast32_t> states;
        for (auto& end_state : end_states) {
            for (auto& t_in : to_from_connections[end_state]) {
                states.insert(t_in.from);
            }
            to_from_connections[end_state].clear();
        }

        // Redirect transitions to end states towards the new final state
        for (auto& state : states) {
            size_t transition_n = 0;
            while (transition_n < from_to_connections[state].size()) {
                auto t = from_to_connections[state][transition_n];
                if (end_states.find(t.to) != end_states.end()) {
                    auto data_checks = std::vector<std::tuple<Operators, std::string, QueryElement>>();
                    for (size_t i = 0; i < t.property_checks.size(); i++) {
                        data_checks.push_back(std::tuple<Operators, std::string, QueryElement>(t.property_checks[i]));
                    }
                    add_transition(RDPQTransition::make_data_transition(t.from, final_state, move(data_checks)));

                    // Delete old transition to end state
                    from_to_connections[state].erase(
                        from_to_connections[state].begin() + transition_n);
                } else {
                    transition_n++;
                }
            }
        }

        // New state is now the only end state
        end_states.clear();
        end_states.insert(final_state);
    }
}

// ----- Auxiliary methods -----


// Distance from every state to the final state
void RDPQAutomaton::calculate_distance_to_final_state() {
    // BFS starting from the final_state
    queue<pair<uint32_t, uint32_t>> open;
    set<uint32_t> visited;
    for (uint32_t i = 0; i < total_states; i++) {
        distance_to_final.push_back(UINT32_MAX);
    }
    open.push(make_pair(final_state, 0));
    while (!open.empty()) {
        auto current_pair = open.front();
        auto current_state = current_pair.first;
        auto current_distance = current_pair.second;
        open.pop();
        if (visited.find(current_state) == visited.end()) {
            visited.insert(current_state);
            // Set distance to final_state. BFS guarantees that this distance
            // is the minimum, considering that data transitions do not count as distance.
            distance_to_final[current_state] = current_distance;
            for (const auto& transition : to_from_connections[current_state]) {
                auto distance = current_distance;
                if (!transition.is_check) {
                    distance++;
                }
                open.push(make_pair(transition.from, distance));
            }
        }
    }
}


// Sort transitions according to distance to final state
void RDPQAutomaton::sort_transitions() {
    // Assume that distance_to_final is already computed
    for (size_t state = 0; state < from_to_connections.size(); state++) {
        sort_state_transition(state);
    }
}


// Sort transitions according to distance to final state (in a specific state)
void RDPQAutomaton::sort_state_transition(uint32_t state) {
    for (size_t i = 0; i < from_to_connections[state].size(); i++) {
        auto i_next_state = from_to_connections[state][i].to;
        size_t min_distance = distance_to_final[i_next_state];
        size_t min_index = i;
        for (size_t j = i + 1; j < from_to_connections[state].size(); j++) {
            auto j_next_state = from_to_connections[state][j].to;
            if (distance_to_final[j_next_state] < min_distance) {
                min_distance = distance_to_final[j_next_state];
                min_index = j;
            }
        }
        if (min_index != i) {
            auto aux = from_to_connections[state][i];
            from_to_connections[state][i] = from_to_connections[state][min_index];
            from_to_connections[state][min_index] = aux;
        }
    }
}


// RDPQAutomaton RDPQAutomaton::invert_automaton() {
//     auto automaton_inverted = RDPQAutomaton();

//     // Add all transitions of the current automaton, but change the from and to
//     for (size_t state = 0; state < from_to_connections.size(); state++) {
//         for (auto& transition : from_to_connections[state]) {
//             auto data_checks = std::vector<std::tuple<Operators, std::string, QueryElement>>();
//             for (size_t i = 0; i < transition.property_checks.size(); i++) {
//                 data_checks.push_back(std::tuple<Operators, std::string, QueryElement>(transition.property_checks[i]));
//             }
//             if (transition.is_check) {
//                 automaton_inverted.add_transition(RDPQTransition::make_data_transition(
//                     transition.to,
//                     transition.from,
//                     move(data_checks)
//                 ));
//             } else {
//                 automaton_inverted.add_transition(RDPQTransition::make_edge_transition(
//                     transition.to,
//                     transition.from,
//                     transition.inverse,
//                     transition.type,
//                     move(data_checks)
//                 ));
//             }

//         }
//     }

//     automaton_inverted.start = final_state;
//     automaton_inverted.final_state = start;
//     return automaton_inverted;
// }
