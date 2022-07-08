#pragma once

#include <algorithm>

#include "parser/query/paths/path.h"

class PathAtom : public IPath {
public:
    std::string atom;
    bool inverse;
    std::vector<std::tuple<Operators, std::string, QueryElement>> property_checks;

    PathAtom(std::string atom, bool inverse,
             std::vector<std::tuple<Operators, std::string, QueryElement>>&& property_checks
               = std::vector<std::tuple<Operators, std::string, QueryElement>>()) :
        atom    (atom),
        inverse (inverse),
        property_checks (move(property_checks)) { }

    PathAtom(const PathAtom& other) :
        atom    (other.atom),
        inverse (other.inverse),
        property_checks (other.property_checks) { }

    PathType type() const override {
        return PathType::PATH_ATOM;
    }

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

    std::unique_ptr<IPath> invert() const override {
        auto data_checks = std::vector<std::tuple<Operators, std::string, QueryElement>>();
        for (size_t i = 0; i < property_checks.size(); i++) {
            data_checks.push_back(std::tuple<Operators, std::string, QueryElement>(property_checks[i]));
        }
        return std::make_unique<PathAtom>(atom, !inverse, move(data_checks));
    }

    bool nullable() const override {
        return false;
    }

    std::unique_ptr<IPath> duplicate() const override {
        auto data_checks = std::vector<std::tuple<Operators, std::string, QueryElement>>();
        for (size_t i = 0; i < property_checks.size(); i++) {
            data_checks.push_back(std::tuple<Operators, std::string, QueryElement>(property_checks[i]));
        }
        return std::make_unique<PathAtom>(atom, inverse, move(data_checks));
    }

    RPQAutomaton get_rpq_base_automaton() const override {
        // Create a simple automaton
        auto automaton = RPQAutomaton();
        automaton.end_states.insert(1);
        // Connect states with atom as label
        automaton.add_transition(Transition(0, 1, atom, inverse));
        return automaton;
    }

    RDPQAutomaton get_rdpq_base_automaton() const override {
        // Create a simple automaton
        auto automaton = RDPQAutomaton();

        // Empty data check first (D-state)
        automaton.add_transition(RDPQTransition::make_data_transition(0, 1));

        // Add edge transition (E-state)
        auto data_checks = std::vector<std::tuple<Operators, std::string, QueryElement>>();
        for (size_t i = 0; i < property_checks.size(); i++) {
            data_checks.push_back(std::tuple<Operators, std::string, QueryElement>(property_checks[i]));
        }
        std::sort(data_checks.begin(), data_checks.end());
        data_checks.erase(unique(data_checks.begin(), data_checks.end()), data_checks.end());
        automaton.add_transition(RDPQTransition::make_edge_transition(1, 2, inverse, atom, move(data_checks)));

        // Add another empty data check (D-state)
        automaton.end_states.insert(3);
        automaton.add_transition(RDPQTransition::make_data_transition(2, 3));
        return automaton;
    }
};
