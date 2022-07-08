#pragma once

#include <algorithm>

#include "base/exceptions.h"
#include "parser/query/paths/path.h"

class PathCheck : public IPath {
public:
    std::vector<std::tuple<Operators, std::string, QueryElement>> property_checks;

    PathCheck(std::vector<std::tuple<Operators, std::string, QueryElement>> property_checks) :
        property_checks (move(property_checks)) { }

    PathType type() const override {
        return PathType::PATH_CHECK;
    }

    std::string to_string() const override {
        // TODO:
        std::string check = "";
        return "{" + check + "}";
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ');
        // TODO:
        std::string check = "";
        os << "OpCheck(" << "{" + check + "}" << ")\n";
        return os;
    }

    std::unique_ptr<IPath> invert() const override {
        // Inversion returns the same data check
        return this->duplicate();
    }

    std::unique_ptr<IPath> duplicate() const override {
        auto data_checks = std::vector<std::tuple<Operators, std::string, QueryElement>>();
        for (size_t i = 0; i < property_checks.size(); i++) {
            data_checks.push_back(std::tuple<Operators, std::string, QueryElement>(property_checks[i]));
        }
        return std::make_unique<PathCheck>(move(data_checks));
    }

    bool nullable() const override {
        return false;
    }

    RPQAutomaton get_rpq_base_automaton() const override {
        throw LogicException("Data checks not supported in RPQ");
    }

    // Atomic node data check
    RDPQAutomaton get_rdpq_base_automaton() const override {
        auto automaton = RDPQAutomaton();
        automaton.end_states.insert(1);
        auto data_checks = std::vector<std::tuple<Operators, std::string, QueryElement>>();

        for (size_t i = 0; i < property_checks.size(); i++) {
            data_checks.push_back(std::tuple<Operators, std::string, QueryElement>(property_checks[i]));
        }

        // Remove duplicates for data check
        std::sort(data_checks.begin(), data_checks.end());
        data_checks.erase(unique(data_checks.begin(), data_checks.end()), data_checks.end());

        // Add data check transition
        automaton.add_transition(RDPQTransition::make_data_transition(0, 1, move(data_checks)));
        return automaton;
    }
};
