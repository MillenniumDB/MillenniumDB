/**
 * Validate the RPQ automaton construction for some common regular path expressions.
 */

#include <algorithm>
#include <iostream>
#include <unordered_set>

#include "query/parser/paths/path_alternatives.h"
#include "query/parser/paths/path_atom.h"
#include "query/parser/paths/path_kleene_plus.h"
#include "query/parser/paths/path_kleene_star.h"
#include "query/parser/paths/path_optional.h"
#include "query/parser/paths/path_sequence.h"
#include "query/parser/paths/regular_path_expr.h"

// Generate an unique ObjectId for a string. Uniquely used for this test in order to prevent initializing a GraphModel
ObjectId get_unique_object_id(const std::string& str) {
    static uint64_t                                  current_id = 1;
    static std::unordered_map<std::string, uint64_t> str2id;

    auto it = str2id.find(str);
    if (it != str2id.end())
        return ObjectId(it->second);
    str2id.insert({ str, current_id });

    return ObjectId(current_id++);
}


// Create an RPQ_DFA given a list of transitions, a set of final states and a start state. The states are relabelled
// from 0 to (total_states - 1)
RPQ_DFA create_rpq_dfa(
    std::vector<std::tuple<uint64_t, uint64_t, std::string>> transitions,
    std::unordered_set<uint64_t> final_states)
{
    uint64_t start_state = 0;
    std::unordered_map<uint64_t, uint64_t> state2id { { start_state, 0 } };

    RPQ_DFA dfa;
    dfa.start_state = state2id[start_state];
    dfa.from_to_connections.push_back({});
    dfa.is_final_state.push_back(final_states.find(start_state) != final_states.end());
    dfa.total_final_states = final_states.size();

    for (auto& transition : transitions) {
        auto [from, to, type] = transition;

        bool inverse = false;
        if (type[0] == '^') {
            type    = type.substr(1);
            inverse = true;
        }

        for (auto& state : { from, to }) {
            if (state2id.find(state) == state2id.end()) {
                state2id[state] = dfa.total_states;
                dfa.total_states++;
                dfa.from_to_connections.push_back({});
                dfa.is_final_state.push_back(final_states.find(state) != final_states.end());
            }
        }

        dfa.total_final_states = 0;
        for (const auto& state_is_final : dfa.is_final_state) {
            if (state_is_final)
                ++dfa.total_final_states;
        }

        dfa.from_to_connections[state2id[from]].push_back(
          RPQ_DFA::Transition(state2id[from], state2id[to], get_unique_object_id(type), inverse));
    }

    return dfa;
}


bool check_permutation(const RPQ_DFA& A, const RPQ_DFA& B, uint_fast32_t* mapping) {
    for (uint_fast32_t s = 0; s < A.total_states; s++) {
        if (A.is_final_state[s] != B.is_final_state[mapping[s]]
            || A.from_to_connections.size() != B.from_to_connections.size())
        {
            return false;
        }

        for (auto& a_transition : A.from_to_connections[s]) {
            bool found_same_transition = false;
            for (auto& b_transition : B.from_to_connections[mapping[s]]) {
                if (mapping[a_transition.from] == b_transition.from
                    && mapping[a_transition.to] == b_transition.to
                    && a_transition.type_id == b_transition.type_id
                    && a_transition.inverse == b_transition.inverse)
                {
                    found_same_transition = true;
                }
            }

            if (!found_same_transition) {
                return false;
            }
        }
    }
    return true;
}

// Returns true if both RPQ_DFA are isomorphic, false otherwise. This performs DFS
// across the first automaton, checking that each transition exists in the other. If the automaton is not isomorphic an
// both automata are printed.
bool isomorphic(const RPQ_DFA& A, const RPQ_DFA& B) {
    if (A.total_states != B.total_states
        || A.total_final_states != B.total_final_states)
    {
        return false;
    }

    // Check total transitions are the same
    uint64_t a_total_transitions = 0;
    uint64_t b_total_transitions = 0;

    for (uint64_t s = 0; s < A.total_states; s++) {
        a_total_transitions += A.from_to_connections.size();
        b_total_transitions += B.from_to_connections.size();
    }

    if (a_total_transitions != b_total_transitions) {
        return false;
    }

    // try all permutations to see if gives the same automaton
    {   // permutation enumeration based on https://www.quickperm.org/01example.php
        uint_fast32_t N = A.total_states;
        uint_fast32_t* a = new uint_fast32_t[N];
        uint_fast32_t* p = new uint_fast32_t[N +1];

        for (uint32_t i = 0; i < N; i++) {  // initialize arrays; a[N] can be any type
            a[i] = i;
            p[i] = i;
        }
        p[N] = N; // p[N] > 0 controls iteration and the index boundary for i
        if (check_permutation(A, B, a)) {
            delete[] a;
            delete[] p;
            return true;
        }
        uint_fast32_t tmp;

        // setup first swap points to be 1 and 0 respectively (i & j)
        for (uint_fast32_t i = 1; i < N; ) {
            p[i]--;             // decrease index "weight" for i by one
            auto j = i % 2 * p[i];   // IF i is odd then j = p[i] otherwise j = 0
            tmp = a[j];         // swap(a[j], a[i])
            a[j] = a[i];
            a[i] = tmp;
            if (check_permutation(A, B, a)) {
                delete[] a;
                delete[] p;
                return true;
            }
            i = 1;              // reset index i to 1 (assumed)
            while (!p[i]) {      // while (p[i] == 0)
                p[i] = i;        // reset p[i] zero value
                i++;             // set new index value for i (increase by one)
            }
        }
        delete[] a;
        delete[] p;
    }

    return false;
}


// Checks isomorphism and handles printing errors
bool check(const RPQ_DFA& A, const RPQ_DFA& B) {
    if (!isomorphic(A, B)) {
        std::cout << "Automatons are not isomorphic!\n";
        std::cout << "Got:\n";
        A.print(std::cout);
        std::cout << "\nExpected:\n";
        B.print(std::cout);
        std::cout << "\n";
        return false;
    }

    return true;
}


// Validate the construction of the path kleene plus over a simple path expression.
// The result should be the following RPQ:
//
//            +-------+
//            V       |
// (0)-[a]->((1))-[a]-+
bool simple_kleene_plus() {
    std::unique_ptr<RegularPathExpr> tmp;
    tmp = std::make_unique<PathAtom>("a", false);
    tmp = std::make_unique<PathKleenePlus>(std::move(tmp));

    auto dfa          = tmp->get_rpq_automaton(&get_unique_object_id);
    auto expected_dfa = create_rpq_dfa({ { 0, 1, "a" }, { 1, 1, "a" } }, { 1 });

    return check(dfa, expected_dfa);
}


// Validate the construction of the path kleene star over a simple path expression.
// The result should be the following RPQ:
//
//   +--------+
//   V        |
// ((0))-[a]--+
bool simple_kleene_star() {
    std::unique_ptr<RegularPathExpr> tmp;
    tmp = std::make_unique<PathAtom>("a", false);
    tmp = std::make_unique<PathKleeneStar>(std::move(tmp));

    auto dfa          = tmp->get_rpq_automaton(&get_unique_object_id);
    auto expected_dfa = create_rpq_dfa({ { 0, 0, "a" } }, { 0 });
    return check(dfa, expected_dfa);
}


// Validate the construction of the path optional over a simple path expression.
// The result should be the following RPQ:
//
// ((0))-[a]->((1))
bool simple_optional() {
    std::unique_ptr<RegularPathExpr> tmp;
    tmp = std::make_unique<PathAtom>("a", false);
    tmp = std::make_unique<PathOptional>(std::move(tmp));

    auto dfa          = tmp->get_rpq_automaton(&get_unique_object_id);
    auto expected_dfa = create_rpq_dfa({ { 0, 1, "a" } }, { 0, 1 });
    return check(dfa, expected_dfa);
}


// Validate that applying kleene plus twice is simplified correctly.
// (A+)+
bool simple_double_kleene_plus() {
    std::unique_ptr<RegularPathExpr> tmp;
    tmp = std::make_unique<PathAtom>("a", false);
    tmp = std::make_unique<PathKleenePlus>(std::move(tmp));
    tmp = std::make_unique<PathKleenePlus>(std::move(tmp));

    auto dfa          = tmp->get_rpq_automaton(&get_unique_object_id);
    auto expected_dfa = create_rpq_dfa({ { 0, 1, "a" }, { 1, 1, "a" } }, { 1 });
    return check(dfa, expected_dfa);
}


// Validate the denull case (A*)* => A*
bool denull_double_kleene_star() {
    std::unique_ptr<RegularPathExpr> tmp;
    tmp = std::make_unique<PathAtom>("a", false);
    tmp = std::make_unique<PathKleeneStar>(std::move(tmp));
    tmp = std::make_unique<PathKleeneStar>(std::move(tmp));

    auto dfa          = tmp->get_rpq_automaton(&get_unique_object_id);
    auto expected_dfa = create_rpq_dfa({ { 0, 0, "a" } }, { 0 });
    return check(dfa, expected_dfa);
}


// Validate the denull case (A*)? => A*
bool denull_optional_kleene_star() {
    std::unique_ptr<RegularPathExpr> tmp;
    tmp = std::make_unique<PathAtom>("a", false);
    tmp = std::make_unique<PathKleeneStar>(std::move(tmp));
    tmp = std::make_unique<PathOptional>(std::move(tmp));

    auto dfa          = tmp->get_rpq_automaton(&get_unique_object_id);
    auto expected_dfa = create_rpq_dfa({ { 0, 0, "a" } }, { 0 });
    return check(dfa, expected_dfa);
}


// Validate the denull case (A?)* => A*
bool denull_kleene_star_optional() {
    std::unique_ptr<RegularPathExpr> tmp;
    tmp = std::make_unique<PathAtom>("a", false);
    tmp = std::make_unique<PathOptional>(std::move(tmp));
    tmp = std::make_unique<PathKleeneStar>(std::move(tmp));

    auto dfa          = tmp->get_rpq_automaton(&get_unique_object_id);
    auto expected_dfa = create_rpq_dfa({ { 0, 0, "a" } }, { 0 });
    return check(dfa, expected_dfa);
}


// Validate the denull case (A*/B*)* => (A|B)*
bool denull_kleene_star_sequence_of_kleene_stars() {
    std::vector<std::unique_ptr<RegularPathExpr>> sequence;
    std::unique_ptr<RegularPathExpr>              tmp;
    tmp = std::make_unique<PathAtom>("a", false);
    tmp = std::make_unique<PathKleeneStar>(std::move(tmp));
    sequence.push_back(std::move(tmp));
    tmp = std::make_unique<PathAtom>("b", false);
    tmp = std::make_unique<PathKleeneStar>(std::move(tmp));
    sequence.push_back(std::move(tmp));
    tmp = std::make_unique<PathSequence>(std::move(sequence));
    tmp = std::make_unique<PathKleeneStar>(std::move(tmp));

    auto dfa = tmp->get_rpq_automaton(&get_unique_object_id);
    auto expected_dfa = create_rpq_dfa(
        { { 0, 0, "a" }, { 0, 0, "b" } },
        { 0 }
    );
    return check(dfa, expected_dfa);
}


// Validate the denull case (A*|B*)* => (A|B)*
bool denull_kleene_star_alternatives_of_kleene_stars() {
    std::vector<std::unique_ptr<RegularPathExpr>> sequence;
    std::unique_ptr<RegularPathExpr>              tmp;
    tmp = std::make_unique<PathAtom>("a", false);
    tmp = std::make_unique<PathKleeneStar>(std::move(tmp));
    sequence.push_back(std::move(tmp));
    tmp = std::make_unique<PathAtom>("b", false);
    tmp = std::make_unique<PathKleeneStar>(std::move(tmp));
    sequence.push_back(std::move(tmp));
    tmp = std::make_unique<PathAlternatives>(std::move(sequence));
    tmp = std::make_unique<PathKleeneStar>(std::move(tmp));

    auto dfa = tmp->get_rpq_automaton(&get_unique_object_id);
    auto expected_dfa = create_rpq_dfa(
        { { 0, 0, "a" }, { 0, 0, "b" } },
        { 0 }
    );
    return check(dfa, expected_dfa);
}

// Validate the denull case (A*|B?)? => A*|B?
bool denull_optional_alternatives_kleene_star_optional() {
    std::vector<std::unique_ptr<RegularPathExpr>> alternatives;
    std::unique_ptr<RegularPathExpr>              tmp;
    tmp = std::make_unique<PathAtom>("a", false);
    tmp = std::make_unique<PathKleeneStar>(std::move(tmp));
    alternatives.push_back(std::move(tmp));
    tmp = std::make_unique<PathAtom>("b", false);
    tmp = std::make_unique<PathOptional>(std::move(tmp));
    alternatives.push_back(std::move(tmp));
    tmp = std::make_unique<PathAlternatives>(std::move(alternatives));
    tmp = std::make_unique<PathOptional>(std::move(tmp));

    auto dfa          = tmp->get_rpq_automaton(&get_unique_object_id);
    auto expected_dfa = create_rpq_dfa({ { 0, 1, "a" }, { 0, 2, "b" }, { 1, 1, "a" } }, { 0, 1, 2 });
    return check(dfa, expected_dfa);
}


// (A|B)+
bool plus_alternatives() {
    auto a = std::make_unique<PathAtom>("a", false);
    auto b = std::make_unique<PathAtom>("b", false);

    std::vector<std::unique_ptr<RegularPathExpr>> alt_list;
    alt_list.push_back(std::move(a));
    alt_list.push_back(std::move(b));
    auto alternatives = std::make_unique<PathAlternatives>(std::move(alt_list));
    auto alternative_plus = std::make_unique<PathKleenePlus>(std::move(alternatives));

    auto dfa          = alternative_plus->get_rpq_automaton(&get_unique_object_id);
    auto expected_dfa = create_rpq_dfa(
        {
            { 0, 1, "a" },
            { 0, 1, "b" },
            { 1, 1, "a" },
            { 1, 1, "b" },
        },
        { 1 });
    return check(dfa, expected_dfa);
}

// (A+)|(B+)
bool alternatives_plus() {
    auto a = std::make_unique<PathAtom>("a", false);
    auto b = std::make_unique<PathAtom>("b", false);

    auto plus_a = std::make_unique<PathKleenePlus>(std::move(a));
    auto plus_b = std::make_unique<PathKleenePlus>(std::move(b));


    std::vector<std::unique_ptr<RegularPathExpr>> alt_list;
    alt_list.push_back(std::move(plus_a));
    alt_list.push_back(std::move(plus_b));
    auto alternatives = std::make_unique<PathAlternatives>(std::move(alt_list));

    auto dfa          = alternatives->get_rpq_automaton(&get_unique_object_id);
    auto expected_dfa = create_rpq_dfa(
        {
            { 0, 1, "a" },
            { 1, 1, "a" },
            { 0, 2, "b" },
            { 2, 2, "b" },
        },
        { 1, 2 });
    return check(dfa, expected_dfa);
}


// (A*)|(B*)
bool alternatives_star() {
    auto a = std::make_unique<PathAtom>("a", false);
    auto b = std::make_unique<PathAtom>("b", false);

    auto star_a = std::make_unique<PathKleeneStar>(std::move(a));
    auto star_b = std::make_unique<PathKleeneStar>(std::move(b));


    std::vector<std::unique_ptr<RegularPathExpr>> alt_list;
    alt_list.push_back(std::move(star_a));
    alt_list.push_back(std::move(star_b));
    auto alternatives = std::make_unique<PathAlternatives>(std::move(alt_list));

    auto dfa          = alternatives->get_rpq_automaton(&get_unique_object_id);
    auto expected_dfa = create_rpq_dfa(
        {
            { 0, 1, "a" },
            { 1, 1, "a" },
            { 0, 2, "b" },
            { 2, 2, "b" },
        },
        { 0, 1 ,2 });
    return check(dfa, expected_dfa);
}

// (A*)/(B*)
bool sequence_star() {
    auto a = std::make_unique<PathAtom>("a", false);
    auto b = std::make_unique<PathAtom>("b", false);

    auto star_a = std::make_unique<PathKleeneStar>(std::move(a));
    auto star_b = std::make_unique<PathKleeneStar>(std::move(b));


    std::vector<std::unique_ptr<RegularPathExpr>> list;
    list.push_back(std::move(star_a));
    list.push_back(std::move(star_b));
    auto sequence = std::make_unique<PathSequence>(std::move(list));

    auto dfa          = sequence->get_rpq_automaton(&get_unique_object_id);
    auto expected_dfa = create_rpq_dfa(
        {
            { 0, 0, "a" },
            { 0, 1, "b" },
            { 1, 1, "b" },
        },
        { 0, 1 });
    return check(dfa, expected_dfa);
}

// a/b
bool sequence2() {
    auto a = std::make_unique<PathAtom>("a", false);
    auto b = std::make_unique<PathAtom>("b", false);

    std::vector<std::unique_ptr<RegularPathExpr>> list;
    list.push_back(std::move(a));
    list.push_back(std::move(b));
    auto sequence = std::make_unique<PathSequence>(std::move(list));

    auto dfa          = sequence->get_rpq_automaton(&get_unique_object_id);
    auto expected_dfa = create_rpq_dfa(
        {
            { 0, 1, "a" },
            { 1, 2, "b" },
        },
        { 2 });
    return check(dfa, expected_dfa);
}

// a/b
bool sequence3() {
    auto a = std::make_unique<PathAtom>("a", false);
    auto b = std::make_unique<PathAtom>("b", false);
    auto c = std::make_unique<PathAtom>("c", false);

    std::vector<std::unique_ptr<RegularPathExpr>> list;
    list.push_back(std::move(a));
    list.push_back(std::move(b));
    list.push_back(std::move(c));
    auto sequence = std::make_unique<PathSequence>(std::move(list));

    auto dfa          = sequence->get_rpq_automaton(&get_unique_object_id);
    auto expected_dfa = create_rpq_dfa(
        {
            { 0, 1, "a" },
            { 1, 2, "b" },
            { 2, 3, "c" },
        },
        { 3 });
    return check(dfa, expected_dfa);
}

// a|b
bool alternative2() {
    auto a = std::make_unique<PathAtom>("a", false);
    auto b = std::make_unique<PathAtom>("b", false);

    std::vector<std::unique_ptr<RegularPathExpr>> list;
    list.push_back(std::move(a));
    list.push_back(std::move(b));
    auto alt = std::make_unique<PathAlternatives>(std::move(list));

    auto dfa          = alt->get_rpq_automaton(&get_unique_object_id);
    auto expected_dfa = create_rpq_dfa(
        {
            { 0, 1, "a" },
            { 0, 1, "b" },
        },
        { 1 });
    return check(dfa, expected_dfa);
}


// a|b|c
bool alternative3() {
    auto a = std::make_unique<PathAtom>("a", false);
    auto b = std::make_unique<PathAtom>("b", false);
    auto c = std::make_unique<PathAtom>("b", false);

    std::vector<std::unique_ptr<RegularPathExpr>> list;
    list.push_back(std::move(a));
    list.push_back(std::move(b));
    list.push_back(std::move(c));
    auto alt = std::make_unique<PathAlternatives>(std::move(list));

    auto dfa          = alt->get_rpq_automaton(&get_unique_object_id);
    auto expected_dfa = create_rpq_dfa(
        {
            { 0, 1, "a" },
            { 0, 1, "b" },
            { 0, 1, "c" },
        },
        { 1 });
    return check(dfa, expected_dfa);
}


// (a|b)/(c|d)
bool mix1() {
    auto a = std::make_unique<PathAtom>("a", false);
    auto b = std::make_unique<PathAtom>("b", false);
    auto c = std::make_unique<PathAtom>("c", false);
    auto d = std::make_unique<PathAtom>("d", false);

    std::vector<std::unique_ptr<RegularPathExpr>> list1;
    list1.push_back(std::move(a));
    list1.push_back(std::move(b));

    std::vector<std::unique_ptr<RegularPathExpr>> list2;
    list2.push_back(std::move(c));
    list2.push_back(std::move(d));

    auto opt1 = std::make_unique<PathAlternatives>(std::move(list1));
    auto opt2 = std::make_unique<PathAlternatives>(std::move(list2));

    std::vector<std::unique_ptr<RegularPathExpr>> list3;
    list3.push_back(std::move(opt1));
    list3.push_back(std::move(opt2));

    auto seq = std::make_unique<PathSequence>(std::move(list3));

    auto dfa          = seq->get_rpq_automaton(&get_unique_object_id);
    auto expected_dfa = create_rpq_dfa(
        {
            { 0, 1, "a" },
            { 0, 1, "b" },
            { 1, 2, "c" },
            { 1, 2, "d" },
        },
        { 2 });
    return check(dfa, expected_dfa);
}

// (a/b)|(c/d)
bool mix2() {
    auto a = std::make_unique<PathAtom>("a", false);
    auto b = std::make_unique<PathAtom>("b", false);
    auto c = std::make_unique<PathAtom>("c", false);
    auto d = std::make_unique<PathAtom>("d", false);

    std::vector<std::unique_ptr<RegularPathExpr>> list1;
    list1.push_back(std::move(a));
    list1.push_back(std::move(b));

    std::vector<std::unique_ptr<RegularPathExpr>> list2;
    list2.push_back(std::move(c));
    list2.push_back(std::move(d));

    auto seq1 = std::make_unique<PathSequence>(std::move(list1));
    auto seq2 = std::make_unique<PathSequence>(std::move(list2));

    std::vector<std::unique_ptr<RegularPathExpr>> list3;
    list3.push_back(std::move(seq1));
    list3.push_back(std::move(seq2));

    auto opt = std::make_unique<PathAlternatives>(std::move(list3));

    auto dfa          = opt->get_rpq_automaton(&get_unique_object_id);
    auto expected_dfa = create_rpq_dfa(
        {
            { 0, 1, "a" },
            { 1, 2, "b" },
            { 0, 3, "c" },
            { 3, 2, "d" },
        },
        { 2 });
    return check(dfa, expected_dfa);
}


// (a/b)+|(c/d)*
bool mix3() {
    auto a = std::make_unique<PathAtom>("a", false);
    auto b = std::make_unique<PathAtom>("b", false);
    auto c = std::make_unique<PathAtom>("c", false);
    auto d = std::make_unique<PathAtom>("d", false);

    std::vector<std::unique_ptr<RegularPathExpr>> list1;
    list1.push_back(std::move(a));
    list1.push_back(std::move(b));

    std::vector<std::unique_ptr<RegularPathExpr>> list2;
    list2.push_back(std::move(c));
    list2.push_back(std::move(d));

    auto seq1 = std::make_unique<PathSequence>(std::move(list1));
    auto seq2 = std::make_unique<PathSequence>(std::move(list2));

    std::vector<std::unique_ptr<RegularPathExpr>> list3;
    list3.push_back(std::make_unique<PathKleenePlus>(std::move(seq1)));
    list3.push_back(std::make_unique<PathKleeneStar>(std::move(seq2)));

    auto opt = std::make_unique<PathAlternatives>(std::move(list3));

    auto dfa          = opt->get_rpq_automaton(&get_unique_object_id);
    auto expected_dfa = create_rpq_dfa(
        {
            { 0, 1, "a" },
            { 1, 2, "b" },
            { 2, 1, "a" },
            { 0, 3, "c" },
            { 3, 4, "d" },
            { 4, 3, "c" },
        },
        { 0, 2, 4 });
    return check(dfa, expected_dfa);
}


// (A+)/(B+)
bool sequence_plus() {
    auto a = std::make_unique<PathAtom>("a", false);
    auto b = std::make_unique<PathAtom>("b", false);

    auto plus_a = std::make_unique<PathKleenePlus>(std::move(a));
    auto plus_b = std::make_unique<PathKleenePlus>(std::move(b));


    std::vector<std::unique_ptr<RegularPathExpr>> list;
    list.push_back(std::move(plus_a));
    list.push_back(std::move(plus_b));
    auto sequence = std::make_unique<PathSequence>(std::move(list));

    auto dfa          = sequence->get_rpq_automaton(&get_unique_object_id);
    auto expected_dfa = create_rpq_dfa(
        {
            { 0, 1, "a" },
            { 1, 1, "a" },
            { 1, 2, "b" },
            { 2, 2, "b" },
        },
        { 2 });
    return check(dfa, expected_dfa);
}


int main() {
    using TestFunction = bool(void);

    std::vector<TestFunction*> tests = {
        &simple_kleene_plus,                                // a+
        &simple_kleene_star,                                // a*
        &simple_optional,                                   // a?
        &simple_double_kleene_plus,                         // (a+)+
        &denull_double_kleene_star,                         // (a*)*
        &denull_optional_kleene_star,                       // (a*)?
        &denull_kleene_star_optional,                       // (a?)*
        &denull_kleene_star_sequence_of_kleene_stars,       // (a*/b*)*
        &denull_kleene_star_alternatives_of_kleene_stars,   // (a*|b*)*
        &denull_optional_alternatives_kleene_star_optional, // (a*|b?)?
        &plus_alternatives,                                 // (a|b)+
        &alternatives_plus,                                 // a+|b+
        &alternatives_star,                                 // a*|b*
        &sequence_star,                                     // a*/b*
        &sequence_plus,                                     // a+/b+
        &sequence2,                                         // a/b
        &sequence3,                                         // a/b/c
        &alternative2,                                      // a|b
        &alternative3,                                      // a|b|c
        &mix1,                                              // (a|b)/(c|d)
        &mix2,                                              // (a/b)|(c/d)
        &mix3,                                              // (a/b)+|(c/d)*
    };

    int count = 1;
    for (auto& test_func : tests) {
        if (!test_func()) {
            std::cout << "Test #" << count << " failed!\n";
            return EXIT_FAILURE;
        }
        ++count;
    }

    std::cout << "All (" << tests.size() << ") tests passed!\n";

    return EXIT_SUCCESS;
}
