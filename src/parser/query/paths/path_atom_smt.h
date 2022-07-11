#pragma once

#include <algorithm>

#include "parser/query/paths/path.h"

class PathAtomSMT : public IPath {
public:
    std::string atom;
    bool inverse;

    PathAtomSMT(std::string atom,
                bool inverse
                // , std::vector<std::unique_ptr<Formula>> formulas
                ) :
        atom            (atom),
        inverse         (inverse)
        // , formulas (move(formulas))
        { }

    PathType type() const override {
        return PathType::PATH_ATOM_SMT;
    }

    std::string to_string() const override {
        // TODO: printing formulas might be useful for debugging
        if (inverse) {
            return "^:" + atom;
        }
        return ":" + atom;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ');
        os << "OpAtomSMT(" << (inverse ? "^:" : ":") << atom << ")\n";
        return os;
    }

    std::unique_ptr<IPath> invert() const override {
        // TODO: pass formulas to constructor
        // auto duplicated_formulas = std::vector<std::unique_ptr<Formula>>();
        // for (const auto& formula : formulas) {
        //     duplicated_formulas.push_back(formula->duplicate());
        // }
        return std::make_unique<PathAtomSMT>(atom, !inverse/*, move(duplicated_formulas)*/);
    }

    bool nullable() const override {
        return false;
    }

    std::unique_ptr<IPath> duplicate() const override {
        // TODO: pass formulas to constructor
        // auto duplicated_formulas = std::vector<std::unique_ptr<Formula>>();
        // for (const auto& formula : formulas) {
        //     duplicated_formulas.push_back(formula->duplicate());
        // }
        return std::make_unique<PathAtomSMT>(atom, inverse/*, move(duplicated_formulas)*/);
    }

    RPQAutomaton get_rpq_base_automaton() const override {
        throw LogicException("not supported");
    }

    RDPQAutomaton get_rdpq_base_automaton() const override {
        throw LogicException("not supported");
    }

    SMTAutomaton get_smt_base_automaton() const override {
        auto automaton = SMTAutomaton();
        automaton.end_states.insert(1);

        // TODO: pass duplicated formulas
        // auto duplicated_formulas = std::vector<std::unique_ptr<Formula>>();
        // for (const auto& formula : formulas) {
        //     duplicated_formulas.push_back(formula->duplicate());
        // }
        automaton.add_transition(SMTTransition(0, 1, atom, inverse/*, move(duplicated_formulas)*/));
        return automaton;
    }
};
