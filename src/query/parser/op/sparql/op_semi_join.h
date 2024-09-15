#pragma once

#include "query/parser/op/op.h"

namespace SPARQL {

class OpSemiJoin : public Op {
public:
    std::unique_ptr<Op> lhs;
    std::unique_ptr<Op> rhs;

    OpSemiJoin(std::unique_ptr<Op> lhs, std::unique_ptr<Op> rhs) :
        lhs (std::move(lhs)),
        rhs (std::move(rhs)) { }

    std::unique_ptr<Op> clone() const  override {
        return std::make_unique<OpSemiJoin>(lhs->clone(), rhs->clone());
    }

    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override {
        auto res = lhs->get_all_vars();
        auto rhs_vars = rhs->get_all_vars();
        res.insert(rhs_vars.begin(), rhs_vars.end());
        return res;
    }

    std::set<VarId> get_scope_vars() const override {
        return lhs->get_safe_vars();
    }

    std::set<VarId> get_safe_vars() const override {
        return lhs->get_safe_vars();
    }

    std::set<VarId> get_fixable_vars() const override {
        auto res = lhs->get_fixable_vars();
        auto rhs_vars = rhs->get_fixable_vars();
        res.insert(rhs_vars.begin(), rhs_vars.end());
        return res;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ') << "OpSemiJoin()\n";
        lhs->print_to_ostream(os, indent + 2);
        rhs->print_to_ostream(os, indent + 2);
        return os;
    }
};
} // namespace SPARQL
