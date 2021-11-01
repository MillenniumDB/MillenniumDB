#ifndef BASE__OP_MATCH_H_
#define BASE__OP_MATCH_H_

#include <string>
#include <vector>

#include "base/parser/grammar/query/query_ast.h"
#include "base/parser/logical_plan/op/op.h"

class OpMatch : public Op {
public:
    std::unique_ptr<Op> op;

    OpMatch(std::unique_ptr<Op> op) :
        op (std::move(op)) { }

    ~OpMatch() = default;

    std::ostream& print_to_ostream(std::ostream& os, int indent=0) const override {
        os << std::string(indent, ' ');
        os << "OpMatch()";
        os << "\n";
        return op->print_to_ostream(os, indent + 2);
    };

    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<Var> get_vars() const override {
        return op->get_vars();
    }
};

#endif // BASE__OP_MATCH_H_
