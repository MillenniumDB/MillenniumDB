#include "path_automaton.h"

#include <iostream>
#include <queue>
#include <stack>
#include <utility>

using namespace std;

void PathAutomaton::print() {
     for (size_t i = 0; i < from_to_connections.size(); i++) {
        for (auto& t : from_to_connections[i]) {
            cout << t.from << "=[" << (t.inverse ? "^" : "") << t.type << "]=>" << t.to << "\n";
        }
    }
    cout << "distance to end: \n";
    for (size_t i = 0; i < distance_to_final.size(); i++) {
        cout << i << ":" << distance_to_final[i] << "\n";
    }
    cout << "start is final: " << (start_is_final ? "true" : "false") << "\n";
    cout << "end states: { ";
    for (auto& state : end_states) {
        cout << state << "  ";
    }
    cout << "}\n";
    cout << "start state: " << start << "\n";
    cout << "final state: " << final_state << endl;
}


void PathAutomaton::rename_and_merge(PathAutomaton& other) {
    // Add and rename 'other' states to this automaton

    // Renaming consists in sum the original states
    // quantity to id of other states
    auto initial_states = total_states;
    for (size_t i = 0; i < other.from_to_connections.size(); i++) {
        for (auto& t : other.from_to_connections[i]) {
            // Creates a transitions with renamed states
            auto transition = Transition(
                t.from + initial_states,
                t.to + initial_states, t.type, t.inverse);
            // Add transition to this automaton
            connect(transition);
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


void PathAutomaton::connect(Transition transition) {
    // Check if connections vector has slots to save from and to
    while (from_to_connections.size() <= transition.from ||
           from_to_connections.size() <= transition.to)
        {
            vector<Transition> new_vec;
            from_to_connections.push_back(new_vec);
        }
    while (to_from_connections.size() <= transition.from ||
           to_from_connections.size() <= transition.to)
        {
            vector<Transition> new_vec;
            to_from_connections.push_back(new_vec);
        }

    // Check if a the connections exists
    bool exists = false;
    for (auto& t : from_to_connections[transition.from]) {
        if (transition == t) {
            exists = true;
            break;
        }
    }

    // Add connection only if it doesn't exists
    if (!exists) {
        from_to_connections[transition.from].push_back(transition);
        to_from_connections[transition.to].push_back(transition);
    }
    // Update number of states
    total_states = from_to_connections.size();
}


void PathAutomaton::add_epsilon_transition(uint32_t from, uint32_t to) {
    connect(Transition(from , to, "", false));
}


void PathAutomaton::transform_automaton() {
    // Reduce size by deletion of mergeable states
    delete_mergeable_states();

    // Transform into automaton without epsilon transitions
    delete_epsilon_transitions();

    // Delete states that can no be reached by start state
    delete_unreachable_states();

    // Set of final state
    set_final_state();

    // After set final_state, end_states may be unnecesary
    delete_absortion_states();

    calculate_distance_to_final_state();

    sort_transitions();
}


// ----- Transformation handler methods -----

void PathAutomaton::delete_mergeable_states() {
    bool has_changes = true;
    // Repeat process until there are not mergeable states
    while (has_changes) {
        has_changes = false;
        for (size_t s = 0; s < from_to_connections.size(); s++) {
            // If s only can by reached from v and the transition is epsilon, then v = s
            // from != s to avoid merge a state with itself
            if (to_from_connections[s].size() == 1 &&
                to_from_connections[s][0].type.empty() &&
                to_from_connections[s][0].from != s)
            {
                // If from = start, merge to from avoiding delete start state
                if (to_from_connections[s][0].from == start) {
                    merge_states(to_from_connections[s][0].from, s);
                } else {
                    merge_states(s, to_from_connections[s][0].from);
                }

                has_changes = true;
            }
            // If v only has one transition to s, and it is epsilon, then s = v
            // to != s to avoid merge a state with itself
            if (from_to_connections[s].size() == 1 &&
                from_to_connections[s][0].type.empty() &&
                from_to_connections[s][0].to != s)
            {
                if (from_to_connections[s][0].to == start) {
                    merge_states(from_to_connections[s][0].to, s);
                } else {
                    merge_states(s, from_to_connections[s][0].to);
                }
                has_changes = true;
            }
        }
    }
}


void PathAutomaton::delete_epsilon_transitions() {
    for (size_t state = 0; state < from_to_connections.size(); state++) {
        // For each state, get his epsilon closure
        auto epsilon_closure = get_epsilon_closure(state);
        // If a end state is in closure, then state will be in end_state
        for (const auto s : epsilon_closure) {
            if (end_states.find(s) != end_states.end()) {
                end_states.insert(state);
            }
        }
        // Each epsilon transition of state will be replaced
        // by a non epsilon transition that reachs
        size_t transition_n = 0;
        while (transition_n < from_to_connections[state].size()) {
            // Epsilon transitions has empty type
            if (from_to_connections[state][transition_n].type.empty()) {
                // Before delete transition, connect 'state' to states that
                // states of epsilon closure reaches without a epsilon transition
                for (const auto s : epsilon_closure) {
                    for (const auto& t : from_to_connections[s]) {
                        // Connect only if t is not epsilon transition. Always a state
                        // s connect with another state by a non epsilon transition.
                        if (!t.type.empty()) {
                            connect(Transition(state, t.to, t.type, t.inverse));
                        }
                    }
                }
                // delete epsilon transition
                from_to_connections[state].erase(
                    from_to_connections[state].begin() + transition_n);
            } else {
                transition_n++;
            }
        }
    }
}


void PathAutomaton::delete_unreachable_states() {
    // Get reachable states from start state
    auto reachable_states = get_reachable_states(start, false);
    // Avoid iterate over start state
    for (size_t i = 1; i < from_to_connections.size(); i++) {
        // Check if 'i0 is reachable from start
        if (reachable_states.find(i) == reachable_states.end()) {
            // Delete all transitions from and to 'i' if is not reachable
            from_to_connections[i].clear();
            to_from_connections[i].clear();
            end_states.erase(i);

            // Delete all transitions that includes 'i' state
            for (size_t j = 0; j < from_to_connections.size(); j++) {
                // For each state 'j' check if it reaches to 'i' state
                auto iterator = from_to_connections[j].begin();
                while (iterator != from_to_connections[j].end()) {
                    if (iterator->to == i) {
                        // Delete transition
                        from_to_connections[j].erase(iterator);
                    } else {
                        // Go to next transition
                        iterator++;
                    }
                }
                iterator = to_from_connections[j].begin();
                // For each state 'j', check if it is reachable from 'i'
                while (iterator != to_from_connections[j].end()) {
                    if (iterator->from == i) {
                        // Delete transition
                        to_from_connections[j].erase(iterator);
                    } else {
                        // Go to next transition
                        iterator++;
                    }
                }
            }
        }
    }
}


void PathAutomaton::set_final_state() {
    // Collapses end states to one final state

    // If only has one 'end_state' the transformation is not necessary
    if (end_states.size() == 1) {
        // Set final state id as the only state in end_states
        final_state = *end_states.begin();
        // Check if end state is the same as start
        start_is_final = final_state == start;
    } else {
        // If end_states has more than one state, then creates a extra state
        total_states = from_to_connections.size();
        // The new state will be used as final
        final_state = total_states;
        total_states++;
        // Check if start is in end_state and mark this if it is necessary
        if (end_states.find(start) != end_states.end()) {
            start_is_final = true;
        }
        // Redirect states that reach to end_state to new final state
        for (size_t i = 0; i < from_to_connections.size(); i++) {
            for (size_t j = 0; j < from_to_connections[i].size(); j++) {
                const auto& t = from_to_connections[i][j];
                // Check if transition reachs to end state
                if (end_states.find(t.to) != end_states.end()) {
                    // Redirect t to new state
                    connect(Transition(t.from, final_state, t.type, t.inverse));
                }
            }
        }
    }
}


void PathAutomaton::delete_absortion_states() {
    // Get state that can reach final_state
    auto end_reachable_states = get_reachable_states(final_state, true);
    for (size_t i = 0; i < from_to_connections.size(); i++) {
        // Check if 'i' can reach to end state
        if (end_reachable_states.find(i) == end_reachable_states.end()) {
            // Delete all 'i' transitions
            from_to_connections[i].clear();
            to_from_connections[i].clear();
            // Delete transitions that include 'i' state
            for (size_t j = 0; j < from_to_connections.size(); j++) {
                // Delete transitions that reach to 'i'
                auto iterator = from_to_connections[j].begin();
                while (iterator != from_to_connections[j].end()) {
                    if (iterator->to == i) {
                        from_to_connections[j].erase(iterator);
                    } else {
                        iterator++;
                    }
                }
                iterator = to_from_connections[j].begin();
                // Delete transitions that starts from 'i'
                while (iterator != to_from_connections[j].end()) {
                    if (iterator->from == i) {
                        to_from_connections[j].erase(iterator);
                    } else {
                        iterator++;
                    }
                }
            }
        }
    }
    end_states.clear();
}


// ----- Auxiliary methods -----


set<uint32_t> PathAutomaton::get_epsilon_closure(uint32_t state) {
    // Automaton exploration is with dfs algorithm
    set<uint32_t>  epsilon_closure;
    // It is not necesary to force to state belong to it own epsilon closure
    set<uint32_t> visited;
    stack<uint32_t> open;
    open.push(state);
    uint32_t current_state;
    while (!open.empty()) {
        current_state = open.top();
        open.pop();
        if (visited.find(current_state) == visited.end()) {
            visited.insert(current_state);
            for (auto& transition : from_to_connections[current_state]) {
                if (transition.type.empty()) {
                    epsilon_closure.insert(transition.to);
                    open.push(transition.to);
                }
            }
        }
    }
    return epsilon_closure;
}


set<uint32_t> PathAutomaton::get_reachable_states(uint32_t source, bool inverse) {
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
                to_from_connections[current_state] : from_to_connections[current_state];
            for (const auto& transition : next_states) {
                open.push(inverse ? transition.from : transition.to);
            }
        }
    }
    return reachable_states;
}


void PathAutomaton::merge_states(uint32_t destiny, uint32_t source) {
    if (end_states.find(source) != end_states.end()) {
        end_states.insert(destiny);
    }
    // Redirect source=[x]=>v to destiny=[x]=>v
    for (const auto& t : from_to_connections[source]) {
        if (t.from == t.to) {
            connect(Transition(destiny, destiny, t.type, t.inverse));
        } else {
            connect(Transition(destiny, t.to, t.type, t.inverse));
        }
        // Delete source=[x]=>v
        auto to_iterator = to_from_connections[t.to].begin();
        while (to_iterator != to_from_connections[t.to].end()) {
            if ((*to_iterator) == t) {
                to_from_connections[t.to].erase(to_iterator);
                to_iterator = to_from_connections[t.to].end();
            } else {
                to_iterator++;
            }
        }
    }
    // Redirect v=[x]=>source to v=[x]=>destiny
    for (const auto& t : to_from_connections[source]) {
        if (t.from == t.to) {
            connect(Transition(destiny, destiny, t.type, t.inverse));
        } else {
            connect(Transition(t.from, destiny, t.type, t.inverse));
        }
        // Delete v=[x]=>source
        auto from_iterator = from_to_connections[t.from].begin();
        while (from_iterator != from_to_connections[t.from].end()) {
            if ((*from_iterator) == t) {
                from_to_connections[t.from].erase(from_iterator);
                from_iterator = from_to_connections[t.from].end();
            } else {
                from_iterator++;
            }
        }
    }
    from_to_connections[source].clear();
    to_from_connections[source].clear();
}


void PathAutomaton::calculate_distance_to_final_state() {
    // BFS with initial point the final_state
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
            // is the minimum
            distance_to_final[current_state] = current_distance;
            for (const auto& transition : to_from_connections[current_state]) {
                open.push(make_pair(transition.from, current_distance + 1));
            }
        }
    }
}


void PathAutomaton::sort_transitions() {
    // Assume that distance_to_final is computed
    for (size_t state = 0; state < from_to_connections.size(); state++) {
        sort_state_transition(state);
    }
}


void PathAutomaton::sort_state_transition(uint32_t state) {
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


std::pair<PathAutomaton, PathAutomaton> PathAutomaton::partition_automaton(uint32_t split_state) {
    // This method assumes that split_state is a state that partitions
    // the automata into two smaller automata

    auto left_automaton = PathAutomaton();
    auto right_automaton = PathAutomaton();

    // Execute BFS from start to split_state, when this state is reached, it will
    // not expanded. All the connections that all connections that are reached will
    // be added to left_automaton
    queue<uint32_t> open;
    set<uint32_t> visited;
    open.push(start);
    while (!open.empty()) {
        auto current_state = open.front();
        open.pop();

        // Check if state has not visited yet and split_state transitions will not be
        // added to left_automaton
        if (visited.find(current_state) == visited.end()) {
            visited.insert(current_state);
            auto next_states = from_to_connections[current_state];
            for (const auto& transition : next_states) {
                if (transition.from == split_state && transition.from == transition.to) {
                    left_automaton.connect(transition);
                } else if (transition.from != split_state) {
                    left_automaton.connect(transition);
                    open.push(transition.to);
                }
            }
        }
    }
    // Left_automaton's final_state will be the split_state
    left_automaton.final_state = split_state;

    visited.clear(); // Clear visited to execute a new BFS from split_state

    // Right_automaton will be build using the connections that are reached from BFS
    // algorithm starting from split_state
    open.push(split_state);
    // Make split_state the start of the right_automaton. This allows keep the identifier
    // of the states of the current automaton
    right_automaton.start = split_state;
    while (!open.empty()) {
        auto current_state = open.front();
        open.pop();

        // Check if state has not visited yet
        if (visited.find(current_state) == visited.end()) {
            visited.insert(current_state);
            auto next_states = from_to_connections[current_state];
            for (const auto& transition : next_states) {
                right_automaton.connect(transition);
                open.push(transition.to);
            }
        }
    }
    // Right_automaton's final_state is the same of the current automaton
    right_automaton.final_state = final_state;
    return make_pair(left_automaton, right_automaton);
}


std::pair<PathAutomaton, PathAutomaton>
PathAutomaton::partition_by_transition(uint32_t from, uint32_t to) {
    auto left_automaton = partition_automaton(from).first;
    auto right_automaton = partition_automaton(to).second;
    return make_pair(left_automaton, right_automaton);
}


PathAutomaton PathAutomaton::invert_automaton() {
    auto automaton_inverted = PathAutomaton();

    // Add all transitions of the current automaton, but change the from and to
    for (size_t state = 0; state < from_to_connections.size(); state++) {
        for (auto& transition : from_to_connections[state]) {
            automaton_inverted.connect(Transition(
                transition.to,
                transition.from,
                transition.type,
                transition.inverse
            ));

        }
    }

    automaton_inverted.start_is_final = start_is_final;
    automaton_inverted.start = final_state;
    automaton_inverted.final_state = start;

    return automaton_inverted;
}


bool PathAutomaton::state_partitions_to_automaton(uint32_t state) {
    // Execute two BFS, at the left and right of state, visited cannot
    // have common states
    queue<uint32_t> open;
    set<uint32_t> visited_left;
    set<uint32_t> visited_right;
    open.push(start);
    while(!open.empty()) {
        auto current_state = open.front();
        open.pop();
        if (visited_left.find(current_state) == visited_left.end() && current_state != state) {
            visited_left.insert(current_state);
            auto next_states = from_to_connections[current_state];
            for (const auto& transition : next_states) {
                open.push(transition.to);
            }
        }
    }
    // Start BFS from state
    open.push(state);
    while(!open.empty()) {
        auto current_state = open.front();
        open.pop();
        if (visited_right.find(current_state) == visited_right.end()) {
            if (visited_left.find(current_state) != visited_left.end()) {
                return false;
            }
            visited_right.insert(current_state);
            auto next_states = from_to_connections[current_state];
            for (const auto& transition : next_states) {
                open.push(transition.to);
            }
        }
    }
    return true;
}
