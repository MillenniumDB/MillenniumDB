#ifndef BASE__OP_PATH_ALTERNATIVES_H_
#define BASE__OP_PATH_ALTERNATIVES_H_

#include <memory>
#include <string>
#include <vector>

#include "base/parser/logical_plan/op/op_path.h"
#include "base/parser/logical_plan/op/property_paths/path_automaton.h"

class OpPathAlternatives : public OpPath {
public:
    std::vector<std::unique_ptr<OpPath>> alternatives;
    const bool is_nullable;

    static bool get_nullable(const std::vector<std::unique_ptr<OpPath>>& alternatives) {
        for (const auto& alternative : alternatives) {
            if (alternative->nullable()) {
                return true;
            }
        }
        return false;
    }

    OpPathAlternatives(std::vector<std::unique_ptr<OpPath>> _alternatives) :
        alternatives (std::move(_alternatives)),
        is_nullable  (get_nullable(alternatives))
        { }

    OpPathAlternatives(const OpPathAlternatives& other) :
        is_nullable  (other.is_nullable)
    {
        for (const auto& alternative : other.alternatives) {
            alternatives.push_back(alternative->duplicate());
        }
    }

    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::string to_string() const override {
        std::string alternative_string = "(";
        alternative_string.append(alternatives[0]->to_string());
        for (std::size_t i = 1; i < alternatives.size(); i++) {
            alternative_string.append("|");
            alternative_string.append(alternatives[i]->to_string());
        }
        alternative_string.append(")");
        return alternative_string;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent=0) const override{
        os << std::string(indent, ' ');
        os << "OpAlternatives()\n";

        for (auto& alternative : alternatives) {
            alternative->print_to_ostream(os, indent + 2);
        }

        return os;
    };

    bool nullable() const {
        return is_nullable;
    }

    std::unique_ptr<OpPath> duplicate() const override {
        return std::make_unique<OpPathAlternatives>(*this);
    }

    OpPathType type() const { return OpPathType::OP_PATH_ALTERNATIVES; }

    PathAutomaton get_automaton() const override {
        auto alternative_automaton = PathAutomaton();
        // For each alternative create an automaton
        for (const auto& alternative : alternatives) {
            auto child_automaton = alternative->get_automaton();
            alternative_automaton.rename_and_merge(child_automaton);
            auto start_state = alternative_automaton.get_start();
            // Connects start state with child start
            alternative_automaton.add_epsilon_transition(
                start_state, child_automaton.get_start());
            // Child end state is end state of alternative automaton
            for(const auto& end_state : child_automaton.end_states) {
                alternative_automaton.end_states.insert(end_state);
            }
        }
        return alternative_automaton;
    }

    std::unique_ptr<OpPath> invert() const override {
        std::vector<std::unique_ptr<OpPath>> invert_alternatives;
        for (auto& alternative : alternatives) {
            invert_alternatives.push_back(alternative->invert());
        }
        return std::make_unique<OpPathAlternatives>(move(invert_alternatives));
    }
};

#endif // BASE__OP_PATH_ALTERNATIVES_H_
