#ifndef BASE__OP_ISOLATED_TERM_H_
#define BASE__OP_ISOLATED_TERM_H_

#include "base/parser/logical_plan/op/op.h"
#include "base/parser/logical_plan/node_name.h"

class OpIsolatedTerm : public Op {
public:
    const NodeId term;

    OpIsolatedTerm(NodeId _term) :
        term (std::move(_term)) { }

    ~OpIsolatedTerm() = default;

    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }

    bool operator<(const OpIsolatedTerm& other) const {
        return term < other.term;
    }

    std::set<Var> get_vars() const override { return std::set<Var>(); }

    std::ostream& print_to_ostream(std::ostream& os, int indent=0) const override {
        os << std::string(indent, ' ');
        os << "OpIsolatedTerm(" << term << ")\n";
        return os;
    };
};

#endif // BASE__OP_ISOLATED_TERM_H_
