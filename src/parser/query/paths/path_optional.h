#pragma once

#include "parser/query/paths/path.h"

class PathOptional : public IPath {
public:
    std::unique_ptr<IPath> path;

    PathOptional(std::unique_ptr<IPath> path) : path(move(path)) { }

    PathType type() const override {
        return PathType::PATH_OPTIONAL;
    }

    std::string to_string() const override {
        std::string res = "(";
        res.append(path->to_string());
        res.append(")?");
        return res;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ');
        os << "PathOptional()\n";
        path->print_to_ostream(os, indent + 2);
        return os;
    };

    bool nullable() const override {
        return true;
    }

    std::unique_ptr<IPath> duplicate() const override {
        return std::make_unique<PathOptional>(path->duplicate());
    }

    std::unique_ptr<IPath> invert() const override {
        return std::make_unique<PathOptional>(path->invert());
    }

    RPQAutomaton get_rpq_base_automaton() const override {
        auto automaton = path->get_rpq_base_automaton();
        // Make start state a end state
        automaton.end_states.insert(automaton.get_start());
        return automaton;
    }

    RDPQAutomaton get_rdpq_base_automaton() const override {
        auto automaton = path->get_rdpq_base_automaton();

        // Remove transitions from start to end states (to avoid unnecessary transitions)
        size_t s_transition_n = 0;
        while (s_transition_n < automaton.from_to_connections[automaton.get_start()].size()) {
            auto s_t = automaton.from_to_connections[automaton.get_start()][s_transition_n];
            if (automaton.end_states.find(s_t.to) != automaton.end_states.end()) {
                // Delete unnecessary transition to end state
                automaton.from_to_connections[automaton.get_start()].erase(
                    automaton.from_to_connections[automaton.get_start()].begin() + s_transition_n);
            } else {
                s_transition_n++;
            }
        }
        for (auto end_state = automaton.end_states.begin(); end_state != automaton.end_states.end();) {
            size_t transition_n = 0;
            while (transition_n < automaton.to_from_connections[*end_state].size()) {
                auto t = automaton.to_from_connections[*end_state][transition_n];
                if (t.from == automaton.get_start()) {
                    // Delete unnecessary transition from start state
                    automaton.to_from_connections[*end_state].erase(
                        automaton.to_from_connections[*end_state].begin() + transition_n);
                } else {
                    transition_n++;
                }
            }

            // End state is no longer reachable, remove it
            if (!automaton.to_from_connections[*end_state].size()) {
                end_state = automaton.end_states.erase(end_state);
            } else {
                end_state++;
            }
        }

        // Add new state to allow skipping the pattern
        auto new_state = automaton.get_total_states();
        automaton.add_transition(RDPQTransition::make_data_transition(automaton.get_start(), new_state));
        automaton.end_states.insert(new_state);
        return automaton;
    }
};
