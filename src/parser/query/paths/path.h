#pragma once

#include <memory>
#include <ostream>
#include <string>

#include "parser/query/paths/path_automaton.h"

enum class PathType {
    PATH_ALTERNATIVES,
    PATH_SEQUENCE,
    PATH_KLEENE_STAR,
    PATH_ATOM,
    PATH_OPTIONAL,
};

class IPath {
public:
    virtual ~IPath() = default;

    // true if the path can be empty
    virtual bool nullable() const = 0;

    virtual PathType type() const = 0;

    virtual std::string to_string() const = 0;

    virtual PathAutomaton get_automaton() const = 0;

    virtual std::unique_ptr<IPath> invert() const = 0;

    virtual std::unique_ptr<IPath> duplicate() const = 0;

    virtual std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const = 0;

    friend std::ostream& operator<<(std::ostream& os, const IPath& b) {
        return b.print_to_ostream(os);
    }

    bool operator<(const IPath& other) const {
        return to_string() < other.to_string();
    }

    PathAutomaton get_transformed_automaton() const {
        auto automaton = get_automaton();
        automaton.transform_automaton();
        return automaton;
    }
};
