#pragma once

#include "query/parser/op/op.h"
#include "query/parser/op/mql/graph_pattern/op_basic_graph_pattern.h"

namespace MQL {
class OpInsert : public Op {
public:
    // graph pattern
    std::unique_ptr<OpBasicGraphPattern> bgp;

    OpInsert(std::unique_ptr<OpBasicGraphPattern> bgp) :
        bgp (std::move(bgp)) { }

    virtual std::unique_ptr<Op> clone() const override {
        auto bgp_clone = std::make_unique<OpBasicGraphPattern>(
            *bgp
        );
        return std::make_unique<OpInsert>(std::move(bgp_clone));
    }

    bool read_only() const override { return false; }

    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override {
        return { };
    }

    std::set<VarId> get_scope_vars() const override {
        return { };
    }

    std::set<VarId> get_safe_vars() const override {
        return { };
    }

    std::set<VarId> get_fixable_vars() const override {
        return { };
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ');
        os << "OpInsert()\n";
        bgp->print_to_ostream(os, indent + 2);
        return os;
    }
};
} // namespace MQL
