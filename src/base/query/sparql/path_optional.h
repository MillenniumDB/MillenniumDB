#pragma once

#include <memory>
#include <vector>

#include "base/query/sparql/path.h"

namespace SPARQL {

class PathOptional : public IPath {
public:
    std::unique_ptr<IPath> path;

    PathOptional(std::unique_ptr<IPath> path) : path(std::move(path)) { }

    PathOptional(const PathOptional& other) : path(other.path->duplicate()) { }

    PathType type() const override {
        return PathType::PATH_OPTIONAL;
    }

    std::unique_ptr<IPath> duplicate() const override {
        return std::make_unique<PathOptional>(*this);
    }

    std::string to_string() const override {
        return "(" + path->to_string() + ")?";
    }

    bool nullable() const override {
        return true;
    }

    RPQ_NFA get_rpq_base_automaton() const override {
        auto automaton = path->get_rpq_base_automaton();
        // Make start state a end state
        automaton.end_states.insert(automaton.get_start());
        return automaton;
    }
};
} // namespace SPARQL
