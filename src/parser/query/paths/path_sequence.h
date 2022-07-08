#pragma once

#include <vector>
#include <algorithm>

#include "parser/query/paths/path.h"

class PathSequence : public IPath {
public:
    std::vector<std::unique_ptr<IPath>> sequence;

    const bool is_nullable;

    PathType type() const override {
        return PathType::PATH_SEQUENCE;
    }

    PathSequence(std::vector<std::unique_ptr<IPath>> _sequence) :
        sequence    (std::move(_sequence)),
        is_nullable (get_nullable(sequence)) { }

    PathSequence(const PathSequence& other) :
        is_nullable(other.nullable())
    {
        for (const auto& seq : other.sequence) {
            sequence.push_back(seq->duplicate());
        }
    }

    std::unique_ptr<IPath> invert() const override {
        std::vector<std::unique_ptr<IPath>> invert_sequence;
        for (size_t i = 0; i < sequence.size(); i++) {
            invert_sequence.push_back(sequence[sequence.size() - 1 - i]->invert());
        }
        return std::make_unique<PathSequence>(move(invert_sequence));
    }

    std::string to_string() const override {
        std::string sequence_string = "(";
        sequence_string.append(sequence[0]->to_string());
        for (std::size_t i = 1; i < sequence.size(); i++) {
            sequence_string.append("/");
            sequence_string.append(sequence[i]->to_string());
        }
        sequence_string.append(")");
        return sequence_string;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ');
        os << "PathSequence()\n";
        for (auto& seq : sequence) {
            seq->print_to_ostream(os, indent + 2);
        }
        return os;
    }

    std::unique_ptr<IPath> duplicate() const override {
        return std::make_unique<PathSequence>(*this);
    }

    bool nullable() const override {
        return is_nullable;
    }

    RPQAutomaton get_rpq_base_automaton() const override {
        auto sequence_automaton = sequence[0]->get_rpq_base_automaton();
        // For each sequence child create and automaton
        for (size_t i = 1; i < sequence.size(); i++) {
            auto child_automaton = sequence[i]->get_rpq_base_automaton();
            sequence_automaton.rename_and_merge(child_automaton);
            // Connect end state of sequence automaton to start of child
            for (const auto& end_state : sequence_automaton.end_states) {
                sequence_automaton.add_epsilon_transition(end_state, child_automaton.get_start());
            }
            // Replace sequence automaton's end states by child's end states
            sequence_automaton.end_states = std::move(child_automaton.end_states);
        }
        return sequence_automaton;
    }

    RDPQAutomaton get_rdpq_base_automaton() const override {
        auto sequence_automaton = sequence[0]->get_rdpq_base_automaton();

        // For each sequence child create an automaton
        for (size_t i = 1; i < sequence.size(); i++) {
            auto child_automaton = sequence[i]->get_rdpq_base_automaton();
            sequence_automaton.rename_and_merge(child_automaton);

            // Iterate over current end states & get all states that have transitions towards an end state
            std::set<uint_fast32_t> states;
            for (auto& end_state : sequence_automaton.end_states) {
                for (auto& t_in : sequence_automaton.to_from_connections[end_state]) {
                    states.insert(t_in.from);
                }
                sequence_automaton.to_from_connections[end_state].clear();
            }

            // Add new 'direct' data transitions that skip current sequence end states and the child's start state
            for (auto& state : states) {
                size_t transition_n = 0;
                while (transition_n < sequence_automaton.from_to_connections[state].size()) {
                    auto t = sequence_automaton.from_to_connections[state][transition_n];
                    if (sequence_automaton.end_states.find(t.to) != sequence_automaton.end_states.end()) {
                        // Merge transition for the sequence
                        for (auto& s : sequence_automaton.from_to_connections[child_automaton.get_start()]) {
                            auto data_checks = std::vector<std::tuple<Operators, std::string, QueryElement>>();
                            for (size_t i = 0; i < t.property_checks.size(); i++) {
                                data_checks.push_back(std::tuple<Operators, std::string, QueryElement>(t.property_checks[i]));
                            }
                            for (size_t i = 0; i < s.property_checks.size(); i++) {
                                data_checks.push_back(std::tuple<Operators, std::string, QueryElement>(s.property_checks[i]));
                            }

                            // Remove duplicates for data check
                            std::sort(data_checks.begin(), data_checks.end());
                            data_checks.erase(unique(data_checks.begin(), data_checks.end()), data_checks.end());

                            // Add new 'direct' transition
                            sequence_automaton.add_transition(RDPQTransition::make_data_transition(t.from, s.to, move(data_checks)));
                        }

                        // Delete old transition to end state
                        sequence_automaton.from_to_connections[state].erase(
                            sequence_automaton.from_to_connections[state].begin() + transition_n);
                    } else {
                        transition_n++;
                    }
                }
            }

            // Clear transitions from start state of the child (to maintain the DE property)
            for (auto& t : sequence_automaton.from_to_connections[child_automaton.get_start()]) {
                size_t transition_n = 0;
                while (transition_n < sequence_automaton.to_from_connections[t.to].size()) {
                    if (sequence_automaton.to_from_connections[t.to][transition_n].from == child_automaton.get_start()) {
                        sequence_automaton.to_from_connections[t.to].erase(
                            sequence_automaton.to_from_connections[t.to].begin() + transition_n);
                    } else {
                        transition_n++;
                    }
                }
            }
            sequence_automaton.from_to_connections[child_automaton.get_start()].clear();

            // Replace sequence automaton's end states with child's end states
            sequence_automaton.end_states = move(child_automaton.end_states);
        }

        return sequence_automaton;
    }

private:
    static bool get_nullable(const std::vector<std::unique_ptr<IPath>>& sequence) {
        for (const auto& seq : sequence) {
            if (!seq->nullable()) {
                return false;
            }
        }
        return true;
    }
};
