#pragma once

#include "parser/query/paths/path.h"
#include "parser/query/paths/denull_path.h"

class PathKleeneStar : public IPath {
public:
    std::unique_ptr<IPath> path;

    PathKleeneStar(std::unique_ptr<IPath> _path) :
        path (PathDenull::denull(std::move(_path))) { }

    PathKleeneStar(const PathKleeneStar& other) :
        path (other.path->duplicate()) { }

    std::string to_string() const override {
        std::string res = "(";
        res.append(path->to_string());
        res.append(")*");
        return res;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ');
        os << "PathKleeneStar()\n";
        path->print_to_ostream(os, indent + 2);

        return os;
    };

    bool nullable() const override {
        return true;
    }

    std::unique_ptr<IPath> duplicate() const override {
        return std::make_unique<PathKleeneStar>(*this);
    }

    PathType type() const override {
        return PathType::PATH_KLEENE_STAR;
    }

    PathAutomaton get_automaton() const override {
        auto path_automaton = path->get_automaton();
        // Heuristic for kleene star consutriction
        if (path_automaton.get_total_states() == 2) {
            // Automaton with 2 states have only one connection from 0 to 1
            auto  new_automaton = PathAutomaton();
            auto& transition    = path_automaton.from_to_connections[0][0];
            new_automaton.connect(Transition(0, 0, transition.type, transition.inverse));
            new_automaton.end_states.insert(new_automaton.get_start());
            return new_automaton;
        } else {
            // Connects all end states to start state
            for (const auto& end_state : path_automaton.end_states) {
                path_automaton.add_epsilon_transition(end_state, path_automaton.get_start());
            }
            // Makes start state final
            path_automaton.end_states.insert(path_automaton.get_start());
            return path_automaton;
        }
    }

    std::unique_ptr<IPath> invert() const override {
        return std::make_unique<PathKleeneStar>(path->invert());
    }
};
