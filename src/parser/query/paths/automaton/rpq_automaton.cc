#include "rpq_automaton.h"

#include <iostream>
#include <map>
#include <queue>
#include <stack>
#include <utility>

using namespace std;


void RPQ_NFA::print() {
     for (size_t i = 0; i < outer_transitions.size(); i++) {
        for (auto& t : outer_transitions[i]) {
            if (t.type == nullptr) {
                cout << t.from << "=[ε]=>" << t.to << "\n";
            } else {
                cout << t.from << "=[" << (t.inverse ? "^" : "") << *t.type << "]=>" << t.to << "\n";
            }
        }
    }
    cout << "start state: " << start << "\n";
    cout << "end states: { ";
    for (auto& state : end_states) {
        cout << state << " ";
    }
    cout << "}\n";
}


void RPQ_DFA::print() {
     for (size_t i = 0; i < from_to_connections.size(); i++) {
        for (auto& t : from_to_connections[i]) {
            cout << t.from << "=[" << (t.inverse ? "^" : "") << t.type_id.id << "]=>" << t.to << "\n";
        }
    }
    cout << "start state: " << start_state << "\n";
    cout << "end states: { ";
    for (size_t i = 0; i < total_states; i++) {
        if (is_final_state[i]) {
            cout << i << " ";
        }
    }
    cout << "}\n";
}


void RPQ_NFA::rename_and_merge(RPQ_NFA& other) {
    // Add and rename 'other' states to this automaton

    // Renaming consists in sum the original states
    // quantity to id of other states
    auto initial_states = total_states;
    for (size_t i = 0; i < other.outer_transitions.size(); i++) {
        for (auto& t : other.outer_transitions[i]) {
            // Creates a transitions with renamed states
            auto transition = Transition(t.from + initial_states,
                                         t.to + initial_states, t.type, t.inverse);
            // Add transition to this automaton
            add_transition(transition);
        }
    }

    // Rename 'other' end states
    set<uint32_t> new_end;
    for (auto& end_state : other.end_states) {
        new_end.insert(initial_states + end_state);
    }
    other.end_states = move(new_end);

    // Rename start state
    other.start = initial_states;
}


void RPQ_NFA::add_transition(const Transition& transition) {
    // Check if connections vector has slots to save from and to
    while (outer_transitions.size() <= transition.from ||
           outer_transitions.size() <= transition.to)
    {
        vector<Transition> new_vec;
        outer_transitions.push_back(new_vec);
    }
    while (inner_transitions.size() <= transition.from ||
           inner_transitions.size() <= transition.to)
    {
        vector<Transition> new_vec;
        inner_transitions.push_back(new_vec);
    }

    // Check if a the connections exists
    bool exists = false;
    for (auto& t : outer_transitions[transition.from]) {
        if (transition == t) {
            exists = true;
            break;
        }
    }

    // Add connection only if it doesn't exists
    if (!exists) {
        outer_transitions[transition.from].push_back(transition);
        inner_transitions[transition.to].push_back(transition);
    }
    // Update number of states
    total_states = outer_transitions.size();
}


void RPQ_NFA::add_epsilon_transition(uint32_t from, uint32_t to) {
    add_transition(Transition(from , to, nullptr, false));
}


RPQ_DFA RPQ_NFA::transform_automaton(std::function<ObjectId(const std::string&)> f) {
    delete_mergeable_states();

    delete_epsilon_transitions();

    delete_unreachable_states();

    RPQ_DFA dfa;

    // do dfs
    std::stack<std::vector<bool>> open;
    std::set<std::vector<bool>> visited;
    std::map<std::vector<bool>, size_t> mapping;

    std::vector<bool> start_set(total_states, false);
    start_set[start] = true;
    open.push(start_set);
    mapping.insert({start_set, 0});

    dfa.is_final_state.push_back(end_states.find(start) != end_states.end());
    dfa.from_to_connections.push_back({});

    size_t new_state_count = 1;
    while (!open.empty()) {
        auto current_set = open.top();
        open.pop();

        auto current_state = mapping[current_set]; // must exist

        if (visited.find(current_set) == visited.end()) {
            visited.insert(current_set);

            using type_inverse_t = std::pair<std::string, bool>;
            std::map<type_inverse_t, std::vector<bool>> reached_grouped_by_type;
            std::map<type_inverse_t, bool> reached_grouped_by_type_is_final;

            for (size_t i = 0; i < current_set.size(); i++) {
                if (current_set[i]) {
                    for (auto& transition : outer_transitions[i]) {
                        type_inverse_t type_inverse = { *transition.type, transition.inverse };
                        auto found_type_set = reached_grouped_by_type.find(type_inverse);
                        if (found_type_set == reached_grouped_by_type.end()) {
                            std::vector<bool> new_set(total_states, false);
                            new_set[transition.to] = true;
                            reached_grouped_by_type.insert({ type_inverse, new_set});
                            reached_grouped_by_type_is_final.insert({type_inverse, end_states.find(transition.to) != end_states.end()});
                        } else {
                            auto& existing_set = found_type_set->second;
                            existing_set[transition.to] = true;
                            if (end_states.find(transition.to) != end_states.end()) {
                                reached_grouped_by_type_is_final[type_inverse] = true;;
                            }
                        }
                    }
                }
            }

            for (auto&& [type_inverse, set] : reached_grouped_by_type) {
                auto mapping_found = mapping.find(set);
                if (mapping_found == mapping.end()) {
                    auto new_state = new_state_count++;
                    mapping.insert({set, new_state});

                    dfa.total_states++;
                    dfa.from_to_connections.push_back({});
                    dfa.from_to_connections[current_state].push_back(RPQ_DFA::Transition(current_state, new_state, f(type_inverse.first), type_inverse.second));
                    dfa.is_final_state.push_back(reached_grouped_by_type_is_final[type_inverse]);
                    open.push(set);
                } else {
                    auto existing_state = mapping_found->second;
                    dfa.from_to_connections[current_state].push_back(RPQ_DFA::Transition(current_state, existing_state, f(type_inverse.first), type_inverse.second));
                }

            }
        }
    }

    // Count number of final states
    dfa.total_final_states = 0;
    for (const auto& state_is_final : dfa.is_final_state) {
        if (state_is_final) {
            dfa.total_final_states++;
        }
    }

    // dfa.print();
    return dfa;
}


void RPQ_NFA::delete_mergeable_states() {
    // Repeat process until is certain that there are not mergeable states
    bool repeat_while = true;
    while (repeat_while) {
        repeat_while = false;
        for (size_t s = 0; s < outer_transitions.size(); s++) {
            // If `s` only can by reached from `v` (v = inner_transitions[s][0].from) and the transition is epsilon, then v = s
            // We also check `v != s` to avoid merging a state with itself
            if (inner_transitions[s].size() == 1 &&
                inner_transitions[s][0].type == nullptr &&
                inner_transitions[s][0].from != s)
            {
                // If v is the start state, merge avoiding deleting the start state
                if (inner_transitions[s][0].from == start) {
                    merge_states(inner_transitions[s][0].from, s); // TODO: pensar bien ¿es redundante el orden?
                } else {
                    merge_states(s, inner_transitions[s][0].from);
                }

                repeat_while = true;
            }
            // If `s` has only one transition to `v` (v = outer_transitions[s][0].to), and the transition is epsilon, then s = v
            // We also check `v != s` to avoid merging a state with itself
            if (outer_transitions[s].size() == 1 &&
                outer_transitions[s][0].type == nullptr &&
                outer_transitions[s][0].to != s)
            {
                if (outer_transitions[s][0].to == start) {
                    merge_states(outer_transitions[s][0].to, s);
                } else {
                    merge_states(s, outer_transitions[s][0].to);
                }
                repeat_while = true;
            }
        }
    }
}


void RPQ_NFA::delete_epsilon_transitions() {
    for (size_t state = 0; state < outer_transitions.size(); state++) {
        // For each state, get his epsilon closure
        auto epsilon_closure = get_epsilon_closure(state);
        // If a end state is in closure, then state will be in end_state
        for (const auto s : epsilon_closure) {
            if (end_states.find(s) != end_states.end()) {
                end_states.insert(state);
            }
        }
        // Each epsilon transition of state will be replaced
        // by a non epsilon transition that reaches
        size_t transition_n = 0;
        while (transition_n < outer_transitions[state].size()) {
            // Epsilon transitions has empty type
            if (outer_transitions[state][transition_n].type == nullptr) {
                // Before delete transition, connect 'state' to states that
                // states of epsilon closure reaches without a epsilon transition
                for (const auto s : epsilon_closure) {
                    for (const auto& t : outer_transitions[s]) {
                        // Connect only if t is not epsilon transition. Always a state
                        // s connect with another state by a non epsilon transition.
                        if (t.type != nullptr) {
                            add_transition(Transition(state, t.to, t.type, t.inverse));
                        }
                    }
                }
                // delete epsilon transition
                outer_transitions[state].erase(
                    outer_transitions[state].begin() + transition_n);
            } else {
                transition_n++;
            }
        }
    }
}


void RPQ_NFA::delete_unreachable_states() {
    // Get reachable states from start state
    auto reachable_states = get_reachable_states(start, false);
    // Avoid iterate over start state
    for (size_t i = 1; i < outer_transitions.size(); i++) {
        // Check if 'i0 is reachable from start
        if (reachable_states.find(i) == reachable_states.end()) {
            // Delete all transitions from and to 'i' if is not reachable
            outer_transitions[i].clear();
            inner_transitions[i].clear();
            end_states.erase(i);

            // Delete all transitions that includes 'i' state
            for (size_t j = 0; j < outer_transitions.size(); j++) {
                // For each state 'j' check if it reaches to 'i' state
                auto iterator = outer_transitions[j].begin();
                while (iterator != outer_transitions[j].end()) {
                    if (iterator->to == i) {
                        // Delete transition
                        outer_transitions[j].erase(iterator);
                    } else {
                        // Go to next transition
                        iterator++;
                    }
                }
                iterator = inner_transitions[j].begin();
                // For each state 'j', check if it is reachable from 'i'
                while (iterator != inner_transitions[j].end()) {
                    if (iterator->from == i) {
                        // Delete transition
                        inner_transitions[j].erase(iterator);
                    } else {
                        // Go to next transition
                        iterator++;
                    }
                }
            }
        }
    }
}


// void RPQ_NFA::set_final_state() {
//     // Collapses end states to one final state

//     // If only has one 'end_state' the transformation is not necessary
//     if (end_states.size() == 1) {
//         // Set final state id as the only state in end_states
//         final_state = *end_states.begin();
//         // Check if end state is the same as start
//         start_is_final = final_state == start;
//     } else {
//         // If end_states has more than one state, then creates a extra state
//         total_states = outer_transitions.size();
//         // The new state will be used as final
//         final_state = total_states;
//         total_states++;
//         // Check if start is in end_state and mark this if it is necessary
//         if (end_states.find(start) != end_states.end()) {
//             start_is_final = true;
//         }
//         // Redirect states that reach to end_state to new final state
//         for (size_t i = 0; i < outer_transitions.size(); i++) {
//             for (size_t j = 0; j < outer_transitions[i].size(); j++) {
//                 const auto& t = outer_transitions[i][j];
//                 // Check if transition reaches to end state
//                 if (end_states.find(t.to) != end_states.end()) {
//                     // Redirect t to new state
//                     add_transition(Transition(t.from, final_state, t.type, t.inverse));
//                 }
//             }
//         }
//     }
// }


// void RPQ_NFA::delete_absorbing_states() {
//     // Get state that can reach final_state
//     auto end_reachable_states = get_reachable_states(final_state, true);
//     for (size_t i = 0; i < outer_transitions.size(); i++) {
//         // Check if 'i' can reach to end state
//         if (end_reachable_states.find(i) == end_reachable_states.end()) {
//             // Delete all 'i' transitions
//             outer_transitions[i].clear();
//             inner_transitions[i].clear();
//             // Delete transitions that include 'i' state
//             for (size_t j = 0; j < outer_transitions.size(); j++) {
//                 // Delete transitions that reach to 'i'
//                 auto iterator = outer_transitions[j].begin();
//                 while (iterator != outer_transitions[j].end()) {
//                     if (iterator->to == i) {
//                         outer_transitions[j].erase(iterator);
//                     } else {
//                         iterator++;
//                     }
//                 }
//                 iterator = inner_transitions[j].begin();
//                 // Delete transitions that starts from 'i'
//                 while (iterator != inner_transitions[j].end()) {
//                     if (iterator->from == i) {
//                         inner_transitions[j].erase(iterator);
//                     } else {
//                         iterator++;
//                     }
//                 }
//             }
//         }
//     }
//     end_states.clear();
// }


set<uint32_t> RPQ_NFA::get_epsilon_closure(uint32_t state) {
    // Automaton exploration is with dfs algorithm
    set<uint32_t>  epsilon_closure;
    // It is not necessary to force to state belong to it own epsilon closure
    set<uint32_t> visited;
    stack<uint32_t> open;
    open.push(state);
    while (!open.empty()) {
        auto current_state = open.top();
        open.pop();
        if (visited.find(current_state) == visited.end()) {
            visited.insert(current_state);
            for (auto& transition : outer_transitions[current_state]) {
                if (transition.type == nullptr) {
                    epsilon_closure.insert(transition.to);
                    open.push(transition.to);
                }
            }
        }
    }
    return epsilon_closure;
}


set<uint32_t> RPQ_NFA::get_reachable_states(uint32_t source, bool inverse) {
    // Return all reachable states from 'source'. Get nodes using DFS algorithm
    stack<uint32_t> open;
    set<uint32_t> reachable_states;
    open.push(source);
    while (!open.empty()) {
        auto current_state = open.top();
        open.pop();

        // Check if state has not visited yet
        if (reachable_states.find(current_state) == reachable_states.end()) {
            reachable_states.insert(current_state);
            // Use inverse to get transitions check direction, inverse=true gets transitions
            // of to_from and inverse=false gets transitions of from_to
            auto next_states = inverse ?
                inner_transitions[current_state] : outer_transitions[current_state];
            for (const auto& transition : next_states) {
                open.push(inverse ? transition.from : transition.to);
            }
        }
    }
    return reachable_states;
}


void RPQ_NFA::merge_states(uint32_t destiny, uint32_t source) {
    if (end_states.find(source) != end_states.end()) {
        end_states.insert(destiny);
    }
    // Redirect source=[x]=>v to destiny=[x]=>v
    for (const auto& t : outer_transitions[source]) {
        if (t.from == t.to) {
            add_transition(Transition(destiny, destiny, t.type, t.inverse));
        } else {
            add_transition(Transition(destiny, t.to, t.type, t.inverse));
        }
        // Delete source=[x]=>v
        auto to_iterator = inner_transitions[t.to].begin();
        while (to_iterator != inner_transitions[t.to].end()) {
            if ((*to_iterator) == t) {
                inner_transitions[t.to].erase(to_iterator);
                to_iterator = inner_transitions[t.to].end();
            } else {
                to_iterator++;
            }
        }
    }
    // Redirect v=[x]=>source to v=[x]=>destiny
    for (const auto& t : inner_transitions[source]) {
        if (t.from == t.to) {
            add_transition(Transition(destiny, destiny, t.type, t.inverse));
        } else {
            add_transition(Transition(t.from, destiny, t.type, t.inverse));
        }
        // Delete v=[x]=>source
        auto from_iterator = outer_transitions[t.from].begin();
        while (from_iterator != outer_transitions[t.from].end()) {
            if ((*from_iterator) == t) {
                outer_transitions[t.from].erase(from_iterator);
                from_iterator = outer_transitions[t.from].end();
            } else {
                from_iterator++;
            }
        }
    }
    outer_transitions[source].clear();
    inner_transitions[source].clear();
}


// void RPQ_NFA::calculate_distance_to_final_state() {
//     // BFS with initial point the final_state
//     queue<pair<uint32_t, uint32_t>> open;
//     set<uint32_t> visited;
//     for (uint32_t i = 0; i < total_states; i++) {
//         distance_to_final.push_back(UINT32_MAX);
//     }
//     open.push(make_pair(final_state, 0));
//     while (!open.empty()) {
//         auto current_pair = open.front();
//         auto current_state = current_pair.first;
//         auto current_distance = current_pair.second;
//         open.pop();
//         if (visited.find(current_state) == visited.end()) {
//             visited.insert(current_state);
//             // Set distance to final_state. BFS guarantees that this distance
//             // is the minimum
//             distance_to_final[current_state] = current_distance;
//             for (const auto& transition : inner_transitions[current_state]) {
//                 open.push(make_pair(transition.from, current_distance + 1));
//             }
//         }
//     }
// }


// void RPQ_NFA::sort_transitions() {
//     // Assume that distance_to_final is computed
//     for (size_t state = 0; state < outer_transitions.size(); state++) {
//         sort_state_transition(state);
//     }
// }


// void RPQ_NFA::sort_state_transition(uint32_t state) {
//     for (size_t i = 0; i < outer_transitions[state].size(); i++) {
//         auto i_next_state = outer_transitions[state][i].to;
//         size_t min_distance = distance_to_final[i_next_state];
//         size_t min_index = i;
//         for (size_t j = i + 1; j < outer_transitions[state].size(); j++) {
//             auto j_next_state = outer_transitions[state][j].to;
//             if (distance_to_final[j_next_state] < min_distance) {
//                 min_distance = distance_to_final[j_next_state];
//                 min_index = j;
//             }
//         }
//         if (min_index != i) {
//             auto aux = outer_transitions[state][i];
//             outer_transitions[state][i] = outer_transitions[state][min_index];
//             outer_transitions[state][min_index] = aux;
//         }
//     }
// }


// bool RPQ_NFA::state_partitions_to_automaton(uint32_t state) {
//     // Execute two BFS, at the left and right of state, visited cannot
//     // have common states
//     queue<uint32_t> open;
//     set<uint32_t> visited_left;
//     set<uint32_t> visited_right;
//     open.push(start);
//     while(!open.empty()) {
//         auto current_state = open.front();
//         open.pop();
//         if (visited_left.find(current_state) == visited_left.end() && current_state != state) {
//             visited_left.insert(current_state);
//             auto next_states = outer_transitions[current_state];
//             for (const auto& transition : next_states) {
//                 open.push(transition.to);
//             }
//         }
//     }
//     // Start BFS from state
//     open.push(state);
//     while(!open.empty()) {
//         auto current_state = open.front();
//         open.pop();
//         if (visited_right.find(current_state) == visited_right.end()) {
//             if (visited_left.find(current_state) != visited_left.end()) {
//                 return false;
//             }
//             visited_right.insert(current_state);
//             auto next_states = outer_transitions[current_state];
//             for (const auto& transition : next_states) {
//                 open.push(transition.to);
//             }
//         }
//     }
//     return true;
// }


// std::pair<RPQ_NFA, RPQ_NFA> RPQ_NFA::partition_automaton(uint32_t split_state) {
//     // This method assumes that split_state is a state that partitions
//     // the automata into two smaller automata

//     auto left_automaton = RPQ_NFA();
//     auto right_automaton = RPQ_NFA();

//     // Execute BFS from start to split_state, when this state is reached, it will
//     // not expanded. All the connections that all connections that are reached will
//     // be added to left_automaton
//     queue<uint32_t> open;
//     set<uint32_t> visited;
//     open.push(start);
//     while (!open.empty()) {
//         auto current_state = open.front();
//         open.pop();

//         // Check if state has not visited yet and split_state transitions will not be
//         // added to left_automaton
//         if (visited.find(current_state) == visited.end()) {
//             visited.insert(current_state);
//             auto next_states = outer_transitions[current_state];
//             for (const auto& transition : next_states) {
//                 if (transition.from == split_state && transition.from == transition.to) {
//                     left_automaton.connect(transition);
//                 } else if (transition.from != split_state) {
//                     left_automaton.connect(transition);
//                     open.push(transition.to);
//                 }
//             }
//         }
//     }
//     // Left_automaton's final_state will be the split_state
//     left_automaton.final_state = split_state;

//     visited.clear(); // Clear visited to execute a new BFS from split_state

//     // Right_automaton will be build using the connections that are reached from BFS
//     // algorithm starting from split_state
//     open.push(split_state);
//     // Make split_state the start of the right_automaton. This allows keep the identifier
//     // of the states of the current automaton
//     right_automaton.start = split_state;
//     while (!open.empty()) {
//         auto current_state = open.front();
//         open.pop();

//         // Check if state has not visited yet
//         if (visited.find(current_state) == visited.end()) {
//             visited.insert(current_state);
//             auto next_states = outer_transitions[current_state];
//             for (const auto& transition : next_states) {
//                 right_automaton.connect(transition);
//                 open.push(transition.to);
//             }
//         }
//     }
//     // Right_automaton's final_state is the same of the current automaton
//     right_automaton.final_state = final_state;
//     return make_pair(left_automaton, right_automaton);
// }


// std::pair<RPQ_NFA, RPQ_NFA>
// RPQ_NFA::partition_by_transition(uint32_t from, uint32_t to) {
//     auto left_automaton = partition_automaton(from).first;
//     auto right_automaton = partition_automaton(to).second;
//     return make_pair(left_automaton, right_automaton);
// }