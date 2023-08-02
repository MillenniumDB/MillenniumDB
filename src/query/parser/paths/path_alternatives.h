#pragma once

#include <vector>

#include "query/parser/paths/regular_path_expr.h"

class PathAlternatives : public RegularPathExpr {
public:
    std::vector<std::unique_ptr<RegularPathExpr>> alternatives;

    const bool is_nullable;

    PathAlternatives(std::vector<std::unique_ptr<RegularPathExpr>>&& _alternatives) :
        alternatives (std::move(_alternatives)),
        is_nullable  (get_nullable(alternatives)) { }

    PathAlternatives(const PathAlternatives& other) :
        is_nullable(other.is_nullable)
    {
        for (const auto& alternative : other.alternatives) {
            alternatives.push_back(alternative->clone());
        }
    }

    std::unique_ptr<RegularPathExpr> clone() const override {
        std::vector<std::unique_ptr<RegularPathExpr>> alternatives_clone;
        for (const auto& seq : alternatives) {
            alternatives_clone.push_back(seq->clone());
        }
        return std::make_unique<PathAlternatives>(std::move(alternatives_clone));
    }

    PathType type() const override {
        return PathType::PATH_ALTERNATIVES;
    }

    std::string to_string() const override {
        std::string alternative_string = "(";
        alternative_string.append(alternatives[0]->to_string());
        for (std::size_t i = 1; i < alternatives.size(); i++) {
            alternative_string.append("|");
            alternative_string.append(alternatives[i]->to_string());
        }
        alternative_string.append(")");
        return alternative_string;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ');
        os << "PathAlternatives()\n";
        for (auto& alternative : alternatives) {
            alternative->print_to_ostream(os, indent + 2);
        }
        return os;
    }

    std::unique_ptr<RegularPathExpr> invert() const override {
        std::vector<std::unique_ptr<RegularPathExpr>> invert_alternatives;
        for (auto& alternative : alternatives) {
            invert_alternatives.push_back(alternative->invert());
        }
        return std::make_unique<PathAlternatives>(std::move(invert_alternatives));
    }

    bool nullable() const override {
        return is_nullable;
    }

    RPQ_NFA get_rpq_base_automaton() const override {
        auto alternative_automaton = RPQ_NFA();
        // For each alternative create an automaton
        for (const auto& alternative : alternatives) {
            auto child_automaton = alternative->get_rpq_base_automaton();
            alternative_automaton.rename_and_merge(child_automaton);
            auto start_state = alternative_automaton.get_start();
            // Connects start state with child start
            alternative_automaton.add_epsilon_transition(start_state, child_automaton.get_start());
            // Child end state is end state of alternative automaton
            for (const auto& end_state : child_automaton.end_states) {
                alternative_automaton.end_states.insert(end_state);
            }
        }
        return alternative_automaton;
    }

    RDPQAutomaton get_rdpq_base_automaton() const override {
        auto alternative_automaton = RDPQAutomaton();

        // For each alternative create an automaton
        for (const auto& alternative : alternatives) {
            auto child_automaton = alternative->get_rdpq_base_automaton();
            alternative_automaton.rename_and_merge(child_automaton);

            // Add new 'direct' data transitions that skip the child's start state
            for (auto& t : alternative_automaton.from_to_connections[child_automaton.get_start()]) {
                std::vector<std::tuple<Operators, ObjectId, ObjectId>> data_checks;
                for (size_t i = 0; i < t.property_checks.size(); i++) {
                    data_checks.push_back(t.property_checks[i]);
                }
                alternative_automaton.add_transition(RDPQTransition::make_data_transition(
                    alternative_automaton.get_start(),
                    t.to,
                    std::move(data_checks)
                ));
            }

            // Clear transitions from child's start state (to maintain the DE property)
            for (auto& t : alternative_automaton.from_to_connections[child_automaton.get_start()]) {
                size_t transition_n = 0;
                while (transition_n < alternative_automaton.to_from_connections[t.to].size()) {
                    if (alternative_automaton.to_from_connections[t.to][transition_n].from == child_automaton.get_start()) {
                        alternative_automaton.to_from_connections[t.to].erase(
                            alternative_automaton.to_from_connections[t.to].begin() + transition_n);
                    } else {
                        transition_n++;
                    }
                }
            }
            alternative_automaton.from_to_connections[child_automaton.get_start()].clear();

            // Child end state is an end state of the alternative automaton
            for (const auto& end_state : child_automaton.end_states) {
                alternative_automaton.end_states.insert(end_state);
            }
        }

        return alternative_automaton;
    }

private:
    static bool get_nullable(const std::vector<std::unique_ptr<RegularPathExpr>>& alternatives) {
        for (const auto& alternative : alternatives) {
            if (alternative->nullable()) {
                return true;
            }
        }
        return false;
    }
};
