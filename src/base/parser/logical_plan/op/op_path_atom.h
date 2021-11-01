#ifndef BASE__OP_PATH_ATOM_H_
#define BASE__OP_PATH_ATOM_H_

#include <memory>
#include <string>
#include <iostream>

#include "base/parser/logical_plan/op/property_paths/path_automaton.h"

class OpPathAtom : public OpPath {
public:
    std::string atom;
    bool inverse;

    OpPathAtom(std::string _atom, bool _inverse) :
        atom    (_atom),
        inverse (_inverse) { }

    OpPathAtom(const OpPathAtom& other) :
        atom    (other.atom),
        inverse (other.inverse) { }

    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::string to_string() const override {
        if (inverse) {
            return "^:" + atom;
        }
        return ":" + atom;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent=0) const override{
        os << std::string(indent, ' ');
        os << "OpAtom(" << atom << ",inverse:" << (inverse ? "true" : "false") << ")\n";

        return os;
    };

    bool nullable() const {
        return false;
    }

    std::unique_ptr<OpPath> duplicate() const override {
        return std::make_unique<OpPathAtom>(*this);
    }

    OpPathType type() const { return OpPathType::OP_PATH_ATOM; }

    PathAutomaton get_automaton() const override {
        // Create a simple automaton
        auto automaton = PathAutomaton();
        automaton.end_states.insert(1);
        // Connect states with atom as label
        automaton.connect(Transition(0, 1, atom, inverse));
        return automaton;
    }

    std::unique_ptr<OpPath> invert() const override {
        return std::make_unique<OpPathAtom>(atom, !inverse);
    }
};

#endif // BASE__OP_PATH_ATOM_H_
