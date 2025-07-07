#pragma once

#include "query/parser/op/sparql/op.h"

namespace SPARQL {

class OpUnitTable : public Op {
public:
    std::unique_ptr<Op> clone() const override {
        return std::make_unique<OpUnitTable>();
    }

    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override {
        return {};
    }

    std::set<VarId> get_scope_vars() const override {
        return {};
    }

    std::set<VarId> get_safe_vars() const override {
        return {};
    }

    std::set<VarId> get_fixable_vars() const override {
        return {};
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ') << "OpUnitTable()\n";
        return os;
    }
};
} // namespace SPARQL
