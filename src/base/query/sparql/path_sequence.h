#pragma once

#include <memory>
#include <vector>

#include "base/query/sparql/path.h"

namespace SPARQL {

class PathSequence : public IPath {
public:
    std::vector<std::unique_ptr<IPath>> sequence;

    const bool is_nullable;

    PathSequence(std::vector<std::unique_ptr<IPath>> _sequence) :
        sequence(std::move(_sequence)), is_nullable(get_nullable(sequence)) { }

    PathSequence(const PathSequence& other) : is_nullable(other.nullable()) {
        for (const auto& seq : other.sequence) {
            sequence.push_back(seq->duplicate());
        }
    }

    PathType type() const override {
        return PathType::PATH_SEQUENCE;
    }

    std::unique_ptr<IPath> duplicate() const override {
        return std::make_unique<PathSequence>(*this);
    }

    std::string to_string() const override {
        std::string result = "(";
        for (const auto& seq : sequence) {
            result += seq->to_string() + "/";
        }
        result.pop_back();
        result += ")";
        return result;
    }

    bool nullable() const override {
        return is_nullable;
    }

    RPQ_NFA get_rpq_base_automaton() const override {
        auto sequence_automaton = sequence[0]->get_rpq_base_automaton();
        // For each sequence child create an automaton
        for (size_t i = 1; i < sequence.size(); ++i) {
            auto child_automaton = sequence[i]->get_rpq_base_automaton();
            sequence_automaton.rename_and_merge(child_automaton);
            // Connect end state of sequence automaton to start of child
            for (const auto& end_state : sequence_automaton.end_states) {
                sequence_automaton.add_epsilon_transition(end_state, child_automaton.get_start());
            }
            // Replace sequence automaton's end states with child's end states
            sequence_automaton.end_states = std::move(child_automaton.end_states);
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
} // namespace SPARQL
