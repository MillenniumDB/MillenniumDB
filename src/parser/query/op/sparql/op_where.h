#pragma once

#include "parser/query/op/op.h"

namespace SPARQL {

class OpWhere : public Op {
public:
    std::unique_ptr<Op> op;

    OpWhere(std::unique_ptr<Op> op) : op(std::move(op)) { }

    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<Var> get_vars() const override {
        return op->get_vars();
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ');
        os << "OpWhere()\n";
        return op->print_to_ostream(os, indent + 2);
    };
};
} // namespace SPARQL