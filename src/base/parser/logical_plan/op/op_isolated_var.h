#ifndef BASE__OP_ISOLATED_VAR_H_
#define BASE__OP_ISOLATED_VAR_H_

#include "base/parser/logical_plan/op/op.h"
#include "base/parser/logical_plan/var.h"

class OpIsolatedVar : public Op {
public:
    const Var var;

    OpIsolatedVar(Var _var) :
        var (std::move(_var)) { }

    ~OpIsolatedVar() = default;


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

#endif // BASE__OP_ISOLATED_VAR_H_
