#ifndef BASE__OP_PATH_OPTIONAL_H_
#define BASE__OP_PATH_OPTIONAL_H_

#include <vector>
#include <memory>

#include "base/parser/logical_plan/op/op_path.h"
#include "base/parser/logical_plan/op/property_paths/path_automaton.h"

class OpPathOptional : public OpPath {
public:
    std::unique_ptr<OpPath> path;

    OpPathOptional(std::unique_ptr<OpPath> _path) :
        path (move(_path)) { }

    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::string to_string() const override {
        std::string res = "(";
        res.append(path->to_string());
        res.append(")?");
        return res;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent=0) const override{
        os << std::string(indent, ' ');
        os << "OpPathOptional()\n";
        path->print_to_ostream(os, indent + 2);

        return os;
    };

    bool nullable() const {
        return true;
    }

    std::unique_ptr<OpPath> duplicate() const override {
        return std::make_unique<OpPathOptional>(path->duplicate());
    }

    OpPathType type() const { return OpPathType::OP_PATH_OPTIONAL; }

    PathAutomaton get_automaton() const override {
        auto automaton = path->get_automaton();
        // Make start state a end state
        automaton.end_states.insert(automaton.get_start());
        return automaton;
    }

    std::unique_ptr<OpPath> invert() const override {
        return std::make_unique<OpPathOptional>(path->invert());
    }

};

#endif // BASE__OP_PATH_OPTIONAL_H_
