#pragma once

#include "query/parser/paths/regular_path_expr.h"

class PathKleenePlus : public RegularPathExpr {
public:
    std::unique_ptr<RegularPathExpr> path;

    PathKleenePlus(std::unique_ptr<RegularPathExpr> path) :
        path (std::move(path)) { }

    PathKleenePlus(const PathKleenePlus& other) :
        path (other.path->clone()) { }

    std::unique_ptr<RegularPathExpr> clone() const override {
        return std::make_unique<PathKleenePlus>(*this);
    }

    PathType type() const override {
        return PathType::PATH_KLEENE_PLUS;
    }

    std::string to_string() const override {
        std::string res = "(";
        res.append(path->to_string());
        res.append(")+");
        return res;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ');
        os << "PathKleenePlus()\n";
        path->print_to_ostream(os, indent + 2);
        return os;
    }

    bool nullable() const override {
        return false;
    }

    RPQ_NFA get_rpq_base_automaton() const override {
        auto path_automaton = path->get_rpq_base_automaton();

        // Connects all end states to start state
        for (const auto& end_state : path_automaton.end_states) {
            path_automaton.add_epsilon_transition(end_state, path_automaton.get_start());
        }
        return path_automaton;
    }

    RDPQAutomaton get_rdpq_base_automaton() const override {
        throw std::runtime_error("PathKleenePlus::get_rpq_base_automaton() not implemented");
    }

    std::unique_ptr<RegularPathExpr> invert() const override {
        return std::make_unique<PathKleenePlus>(path->invert());
    }
};
