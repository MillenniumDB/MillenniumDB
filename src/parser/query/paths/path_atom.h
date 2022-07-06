#pragma once

#include "parser/query/paths/path.h"

class PathAtom : public IPath {
public:
    std::string atom;

    bool inverse;

    PathAtom(std::string atom, bool inverse) :
        atom    (atom),
        inverse (inverse) { }

    PathAtom(const PathAtom& other) :
        atom    (other.atom),
        inverse (other.inverse) { }

    std::string to_string() const override {
        if (inverse) {
            return "^:" + atom;
        }
        return ":" + atom;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ');
        os << "OpAtom(" << (inverse ? "^:" : ":") << atom << ")\n";
        return os;
    }

    bool nullable() const override {
        return false;
    }

    std::unique_ptr<IPath> duplicate() const override {
        return std::make_unique<PathAtom>(*this);
    }

    PathType type() const override {
        return PathType::PATH_ATOM;
    }

    PathAutomaton get_automaton() const override {
        // Create a simple automaton
        auto automaton = PathAutomaton();
        automaton.end_states.insert(1);
        // Connect states with atom as label
        automaton.connect(Transition(0, 1, atom, inverse));
        return automaton;
    }

    std::unique_ptr<IPath> invert() const override {
        return std::make_unique<PathAtom>(atom, !inverse);
    }
};
