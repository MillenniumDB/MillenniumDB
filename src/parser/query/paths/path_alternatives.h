#pragma once

#include <vector>

#include "parser/query/paths/path.h"

class PathAlternatives : public IPath {
public:
    std::vector<std::unique_ptr<IPath>> alternatives;

    const bool is_nullable;

    PathAlternatives(std::vector<std::unique_ptr<IPath>>&& _alternatives) :
        alternatives (std::move(_alternatives)),
        is_nullable  (get_nullable(alternatives)) { }

    PathAlternatives(const PathAlternatives& other) :
        is_nullable(other.is_nullable)
    {
        for (const auto& alternative : other.alternatives) {
            alternatives.push_back(alternative->duplicate());
        }
    }

    static bool get_nullable(const std::vector<std::unique_ptr<IPath>>& alternatives) {
        for (const auto& alternative : alternatives) {
            if (alternative->nullable()) {
                return true;
            }
        }
        return false;
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
    };

    bool nullable() const override {
        return is_nullable;
    }

    std::unique_ptr<IPath> duplicate() const override {
        return std::make_unique<PathAlternatives>(*this);
    }

    PathType type() const override {
        return PathType::PATH_ALTERNATIVES;
    }

    PathAutomaton get_automaton() const override {
        auto alternative_automaton = PathAutomaton();
        // For each alternative create an automaton
        for (const auto& alternative : alternatives) {
            auto child_automaton = alternative->get_automaton();
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

    std::unique_ptr<IPath> invert() const override {
        std::vector<std::unique_ptr<IPath>> invert_alternatives;
        for (auto& alternative : alternatives) {
            invert_alternatives.push_back(alternative->invert());
        }
        return std::make_unique<PathAlternatives>(move(invert_alternatives));
    }
};
