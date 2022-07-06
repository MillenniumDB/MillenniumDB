#pragma once

#include "parser/query/paths/path.h"

class PathOptional : public IPath {
public:
    std::unique_ptr<IPath> path;

    PathOptional(std::unique_ptr<IPath> path) : path(move(path)) { }

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

    PathType type() const override {
        return PathType::PATH_OPTIONAL;
    }

    PathAutomaton get_automaton() const override {
        auto automaton = path->get_automaton();
        // Make start state a end state
        automaton.end_states.insert(automaton.get_start());
        return automaton;
    }

    std::unique_ptr<IPath> invert() const override {
        return std::make_unique<PathOptional>(path->invert());
    }
};
