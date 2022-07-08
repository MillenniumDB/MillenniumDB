#pragma once

#include <memory>
#include <ostream>
#include <string>

#include "parser/query/paths/automaton/rpq_automaton.h"
#include "parser/query/paths/automaton/rdpq_automaton.h"

enum class PathType {
    PATH_ALTERNATIVES,
    PATH_SEQUENCE,
    PATH_KLEENE_STAR,
    PATH_ATOM,
    PATH_OPTIONAL,
    PATH_CHECK,
};

class IPath {
public:
    virtual ~IPath() = default;

    virtual PathType type() const = 0;

    virtual std::string to_string() const = 0;

    virtual std::unique_ptr<IPath> invert() const = 0;

    RPQAutomaton get_rpq_automaton(std::function<ObjectId(const std::string&)> f) const {
        auto automaton = get_rpq_base_automaton();
        automaton.transform_automaton(f);
        return automaton;
    }

    RDPQAutomaton get_rdpq_automaton(std::function<ObjectId(const std::string&)> f) const {
        auto automaton = get_rdpq_base_automaton();
        automaton.transform_automaton(f);
        return automaton;
    }

    friend std::ostream& operator<<(std::ostream& os, const IPath& b) {
        return b.print_to_ostream(os);
    }

    bool operator<(const IPath& other) const {
        return to_string() < other.to_string();
    }

    virtual std::unique_ptr<IPath> duplicate() const = 0;

    virtual std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const = 0;

    // true if the path can be empty
    virtual bool nullable() const = 0;

    virtual RPQAutomaton get_rpq_base_automaton() const = 0;

    virtual RDPQAutomaton get_rdpq_base_automaton() const = 0;
};
