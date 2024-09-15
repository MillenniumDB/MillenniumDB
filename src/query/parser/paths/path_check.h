#pragma once

#include <algorithm>

#include "query/exceptions.h"
#include "graph_models/object_id.h"
#include "query/parser/paths/regular_path_expr.h"

class PathCheck : public RegularPathExpr {
public:
    std::vector<std::tuple<Operators, ObjectId, ObjectId>> property_checks;

    PathCheck(std::vector<std::tuple<Operators, ObjectId, ObjectId>>&& property_checks) :
        property_checks (std::move(property_checks)) { }

    std::unique_ptr<RegularPathExpr> clone() const override {
        auto data_checks = std::vector<std::tuple<Operators, ObjectId, ObjectId>>();
        for (size_t i = 0; i < property_checks.size(); i++) {
            data_checks.push_back(property_checks[i]);
        }
        return std::make_unique<PathCheck>(std::move(data_checks));
    }

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
        std::string check = "";
        os << "OpCheck(" << "{" + this->to_string() + "}" << ")\n";
        return os;
    }

    std::unique_ptr<RegularPathExpr> invert() const override {
        // Inversion returns the same data check
        return this->clone();
    }

    bool nullable() const override {
        return false;
    }

    RPQ_NFA get_rpq_base_automaton() const override {
        throw LogicException("Data checks not supported in RPQ");
    }

    // Atomic node data check
    RDPQAutomaton get_rdpq_base_automaton() const override {
        auto automaton = RDPQAutomaton();
        automaton.end_states.insert(1);
        std::vector<std::tuple<Operators, ObjectId, ObjectId>> data_checks;

        for (size_t i = 0; i < property_checks.size(); i++) {
            data_checks.push_back(property_checks[i]);
        }

        // Remove duplicates for data check
        std::sort(data_checks.begin(), data_checks.end());
        data_checks.erase(unique(data_checks.begin(), data_checks.end()), data_checks.end());

        // Add data check transition
        automaton.add_transition(RDPQTransition::make_data_transition(0, 1, std::move(data_checks)));
        return automaton;
    }
};
