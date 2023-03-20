#pragma once

#include "base/query/sparql/path.h"

namespace SPARQL {

class PathAtom : public IPath {
public:
    Iri  iri;
    bool inverse;

    PathAtom(Iri iri, bool inverse) : iri(iri), inverse(inverse) { }

    PathAtom(const PathAtom& other) : iri(other.iri), inverse(other.inverse) { }

    PathType type() const override {
        return PathType::PATH_ATOM;
    }

    std::unique_ptr<IPath> duplicate() const override {
        return std::make_unique<PathAtom>(*this);
    }

    std::string to_string() const override {
        std::string ret;
        if (inverse) {
            ret += "^";
        }
        ret += "<";
        ret += iri.name;
        ret += ">";
        return ret;
    }

    bool nullable() const override {
        return false;
    }

    RPQ_NFA get_rpq_base_automaton() const override {
        // Create a simple automaton
        auto automaton = RPQ_NFA();
        automaton.end_states.insert(1);
        // Connect states with atom as label
        // ! TODO: Review this (iri as std::string or as SparqlElement Iri)
        automaton.add_transition(RPQ_NFA::Transition(0, 1, &iri.name, inverse));
        return automaton;
    }
};
} // namespace SPARQL
