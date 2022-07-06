#pragma once

#include "base/query/var.h"
#include "parser/query/op/op.h"

class OpIsolatedVar : public Op {
public:
    const Var var;

    OpIsolatedVar(const Var& var) :
        var (var) { }

    OpIsolatedVar(Var&& var) :
        var (std::move(var)) { }

    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }

    bool operator<(const OpIsolatedVar& other) const {
        return var < other.var;
    }

    std::set<Var> get_vars() const override {
        std::set<Var> res;
        res.insert(var);
        return res;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent=0) const override {
        os << std::string(indent, ' ');
        os << "OpIsolatedVar(" << var << ")\n";
        return os;
    };
};
