#pragma once

#include "query/parser/op/mql/op.h"

namespace MQL {

class OpDisjointVar : public Op {
public:
    const VarId var;

    OpDisjointVar(VarId var) :
        var (var) { }

    OpDisjointVar(const OpDisjointVar& other) :
        var (other.var) { }

    std::unique_ptr<Op> clone() const override {
        return std::make_unique<OpDisjointVar>(*this);
    }

    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }

    bool operator<(const OpDisjointVar& other) const {
        return var < other.var;
    }

    std::set<VarId> get_all_vars() const override {
        return { var };
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ');
        os << "OpDisjointVar("
        << get_query_ctx().get_var_name(var)
        << ")\n";
        return os;
    }
};
} // namespace MQL
