#pragma once

#include <memory>
#include <vector>

#include "base/query/sparql/path.h"

namespace SPARQL {

class PathAlternatives : public IPath {
public:
    std::vector<std::unique_ptr<IPath>> alternatives;

    const bool is_nullable;

    PathAlternatives(std::vector<std::unique_ptr<IPath>> _alternatives) :
        alternatives(std::move(_alternatives)), is_nullable(get_nullable(alternatives)) { }

    PathAlternatives(const PathAlternatives& other) : is_nullable(other.nullable()) {
        for (const auto& alt : other.alternatives) {
            alternatives.push_back(alt->duplicate());
        }
    }

    PathType type() const override {
        return PathType::PATH_ALTERNATIVES;
    }

    std::unique_ptr<IPath> duplicate() const override {
        return std::make_unique<PathAlternatives>(*this);
    }

    std::string to_string() const override {
        std::string result = "(";
        for (const auto& alt : alternatives) {
            result += alt->to_string() + "|";
        }
        result.pop_back();
        result += ")";
        return result;
    }

    bool nullable() const override {
        return is_nullable;
    }

    RPQ_NFA get_rpq_base_automaton() const override {
        auto alternative_automaton = RPQ_NFA();
        // For each alternative create an automaton
        for (const auto& alt : alternatives) {
            auto child_automaton = alt->get_rpq_base_automaton();
            alternative_automaton.rename_and_merge(child_automaton);
            auto start_state = alternative_automaton.get_start();
            // Connect start state with child start
            alternative_automaton.add_epsilon_transition(start_state, child_automaton.get_start());
            // Child end state is end state of alternative automaton
            for (const auto& end_state : child_automaton.end_states) {
                alternative_automaton.end_states.insert(end_state);
            }
        }
        return alternative_automaton;
    }

private:
    static bool get_nullable(const std::vector<std::unique_ptr<IPath>>& alternatives) {
        for (const auto& alt : alternatives) {
            if (alt->nullable()) {
                return true;
            }
        }
        return false;
    }
};
} // namespace SPARQL
