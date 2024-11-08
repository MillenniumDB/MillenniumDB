#include "rpq_automaton.h"

#include <cassert>
#include <queue>
#include <map>
#include <stack>
#include <utility>

using namespace std;

struct PartitionSet {
    uint_fast32_t states;

    uint_fast32_t partitions_count;

    // vector of size states, partition[i] gives the partition of the state i
    // the value is between 0 and (partitions_count-1)
    std::vector<uint_fast32_t> partition;

    PartitionSet(const RPQ_DFA& automata) {
        states = automata.total_states;

        // border case where all states are final.
        if (automata.total_final_states == automata.total_states) {
            partitions_count = 1;
            for (uint_fast32_t s = 0; s < states; s++) {
                partition.push_back(0);
            }
            return;
        }

        partitions_count = 2;
        for (uint_fast32_t s = 0; s < states; s++) {
            partition.push_back(automata.is_final_state[s] ? 1 : 0);
        }
    }

    std::vector<uint_fast32_t> get_partition_states(uint_fast32_t partition_number) const {
        std::vector<uint_fast32_t> res;
        for (uint_fast32_t s = 0; s < states; s++) {
            if (partition[s] == partition_number) {
                res.push_back(s);
            }
        }
        return res;
    }
};


struct AlphabetInfo {
    ObjectId oid;
    bool inverse;

    AlphabetInfo() : oid(ObjectId::get_null()), inverse(false) {}

    AlphabetInfo(ObjectId oid, bool inverse) :
        oid(oid), inverse(inverse) { }

    bool operator==(const AlphabetInfo& other) const {
        return oid == other.oid && inverse == other.inverse;
    }

    bool operator<(const AlphabetInfo& other) const {
        if (oid < other.oid) {
            return true;
        } else if (oid > other.oid) {
            return false;
        }
        return inverse < other.inverse;
    }
};


RPQ_DFA RPQ_DFA::optimize(const RPQ_DFA& automata) {
    assert(automata.total_states == automata.from_to_connections.size());
    assert(automata.total_states == automata.is_final_state.size());

    std::set<AlphabetInfo> alphabet_set;

    for (const auto& transitions : automata.from_to_connections) {
        for (const auto& transition : transitions) {
            // std::set will remove duplicates
            alphabet_set.emplace(transition.type_id, transition.inverse);
        }
    }
    std::vector<AlphabetInfo> alphabet;
    for (auto& e : alphabet_set) {
        alphabet.push_back(e);
    }

    uint_fast32_t* transition_table = new uint_fast32_t[automata.total_states * alphabet.size()];

    auto result_transition = [&] (uint_fast32_t state, uint_fast32_t alphabet_idx) {
        return &transition_table[state*alphabet.size() + alphabet_idx];
    };

    for (uint_fast32_t a_i = 0; a_i < alphabet.size(); a_i++) {
        for (uint_fast32_t s = 0; s < automata.total_states; s++) {
            uint_fast32_t reached_state = -1;
            for (auto& transition : automata.from_to_connections[s]) {
                if (transition.inverse == alphabet[a_i].inverse
                    && transition.type_id == alphabet[a_i].oid)
                {
                    reached_state = transition.to;
                }
            }
            *result_transition(s, a_i) = reached_state;
        }
    }

    // create 2 initial partitions, finals and not finals
    PartitionSet current_set(automata);
    PartitionSet next_set(automata);
    do {
        current_set = next_set;
        // we start reconstructing next_set

        int_fast32_t current_result_partition = -1; // will start at zero when ++ is applied
        // iterate over each partition
        for (uint_fast32_t p = 0; p < current_set.partitions_count; p++) {
            auto partition_states = current_set.get_partition_states(p);

            assert(partition_states.size() > 0);

            // first state always defines a new partition
            next_set.partition[partition_states[0]] = ++current_result_partition;

            // Iterate pair of states in partition_states (state_a, state_b)
            // where state_a is before than state_b in partition_states
            for (uint_fast32_t state_b_idx = 1; state_b_idx < partition_states.size(); state_b_idx++) {
                auto state_b = partition_states[state_b_idx];
                bool found_result_partition = false;

                for (uint_fast32_t state_a_idx = 0; state_a_idx < state_b_idx; state_a_idx++) {
                    // see if state_a and state_b are in the same partition in next equivalence
                    bool same_partition = true;
                    auto state_a = partition_states[state_a_idx];
                    for (uint_fast32_t alp_idx = 0; alp_idx < alphabet.size(); alp_idx++) {
                        int_fast32_t reached_a = *result_transition(state_a, alp_idx);
                        int_fast32_t reached_b = *result_transition(state_b, alp_idx);

                        if (reached_a == -1 && reached_b == -1) {
                            continue;
                        } else if (reached_a == -1 || reached_b == -1) {
                            same_partition = false;
                            break;
                        }

                        auto a_res_partition = current_set.partition[reached_a];
                        auto b_res_partition = current_set.partition[reached_b];

                        if (a_res_partition != b_res_partition) {
                            same_partition = false;
                            break;
                        }
                    }
                    if (same_partition) {
                        next_set.partition[state_b] = next_set.partition[state_a];
                        found_result_partition = true;
                        break;
                    }
                }
                if (!found_result_partition) {
                    next_set.partition[state_b] = ++current_result_partition;
                }
            }
        }
        next_set.partitions_count = current_result_partition + 1; // sum one because starts at 0
    } while(current_set.partitions_count < next_set.partitions_count);

    // construct res using current_set
    RPQ_DFA res;

    // each partition becomes a state
    res.start_state = current_set.partition[automata.start_state];
    res.total_states = current_set.partitions_count;
    res.from_to_connections.resize(res.total_states);

    for (uint_fast32_t s = 0; s < automata.total_states; s++) {
        for (const auto& transition : automata.from_to_connections[s]) {
            assert(s == transition.from);
            auto new_from = current_set.partition[transition.from];
            auto new_to   = current_set.partition[transition.to];
            Transition new_transition(
                new_from,
                new_to,
                transition.type_id,
                transition.inverse
            );

            bool already_exists = false;
            for (const auto& existing_transition : res.from_to_connections[new_from]) {
                if (new_transition == existing_transition) {
                    already_exists = true;
                }
            }
            if (!already_exists) {
                res.from_to_connections[new_from].push_back(new_transition);
            }
        }
    }

    // set res.total_final_states and res.is_final_state
    res.is_final_state.resize(res.total_states, false);
    res.total_final_states = 0;

    for (uint_fast32_t s = 0; s < automata.total_states; s++) {
        // consider only if s if final state
        if (!automata.is_final_state[s]) continue;

        auto res_state = current_set.partition[s];

        // maybe a previous s modified res_state, so check
        if (!res.is_final_state[res_state]) {
            res.total_final_states++;
            res.is_final_state[res_state] = true;
        }
    }

    res.add_reverse_connections();

    delete[] transition_table;
    return res;
}


void RPQ_NFA::print(std::ostream& os) {
     for (size_t i = 0; i < outer_transitions.size(); i++) {
        for (auto& t : outer_transitions[i]) {
            if (t.type == nullptr) {
                os << t.from << "=[ε]=>" << t.to << "\n";
            } else {
                os << t.from << "=[" << (t.inverse ? "^" : "") << *t.type << "]=>" << t.to << "\n";
            }
        }
    }
    os << "start state: " << start << "\n";
    os << "end states: { ";
    for (auto& state : end_states) {
        os << state << " ";
    }
    os << "}\n";
}


void RPQ_DFA::print(std::ostream& os) const {
    for (size_t i = 0; i < from_to_connections.size(); i++) {
        for (auto& t : from_to_connections[i]) {
            os << t.from << "=[" << (t.inverse ? "^" : "") << t.type_id.id << "]=>" << t.to << "\n";
        }
    }
    os << "start state: " << start_state << "\n";
    os << "end states: { ";
    for (size_t i = 0; i < total_states; i++) {
        if (is_final_state[i]) {
            os << i << " ";
        }
    }
    os << "}\n";
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
    other.end_states = std::move(new_end);

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


RPQ_DFA RPQ_NFA::transform_automaton(ObjectId(*f)(const std::string&)) {
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
    return RPQ_DFA::optimize(dfa);
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


void RPQ_DFA::add_reverse_connections() {
    reverse_connections.insert(reverse_connections.end(), from_to_connections.size(), {});
    for (size_t state = 0; state < from_to_connections.size(); state++) {
        for (const auto& transition : from_to_connections[state]) {
            // Invert origin/destiny and direction of transition
            reverse_connections[transition.to].push_back(RPQ_DFA::Transition(transition.to, transition.from, transition.type_id, !transition.inverse));
        }
    }
}
