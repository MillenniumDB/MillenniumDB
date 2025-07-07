#pragma once

#include "query/parser/op/sparql/op.h"

namespace SPARQL {

class OpJoin : public Op {
public:
    std::unique_ptr<Op> lhs;
    std::unique_ptr<Op> rhs;

    OpJoin(std::unique_ptr<Op> lhs, std::unique_ptr<Op> rhs) :
        lhs (std::move(lhs)),
        rhs (std::move(rhs)) { }

    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::unique_ptr<Op> clone() const  override {
        return std::make_unique<OpJoin>(lhs->clone(), rhs->clone());
    }

    std::set<VarId> get_all_vars() const override {
        auto res = lhs->get_all_vars();
        auto rhs_vars = rhs->get_all_vars();
        res.insert(rhs_vars.begin(), rhs_vars.end());
        return res;
    }

    std::set<VarId> get_scope_vars() const override {
        auto res = lhs->get_scope_vars();
        auto rhs_vars = rhs->get_scope_vars();
        res.insert(rhs_vars.begin(), rhs_vars.end());
        return res;
    }

    std::set<VarId> get_safe_vars() const override {
        auto res = lhs->get_safe_vars();
        auto rhs_vars = rhs->get_safe_vars();
        res.insert(rhs_vars.begin(), rhs_vars.end());
        return res;
    }

    std::set<VarId> get_fixable_vars() const override {
        auto res = lhs->get_fixable_vars();
        auto rhs_vars = rhs->get_fixable_vars();
        res.insert(rhs_vars.begin(), rhs_vars.end());
        return res;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ') << "OpJoin()\n";
        lhs->print_to_ostream(os, indent + 2);
        rhs->print_to_ostream(os, indent + 2);
        return os;
    }
};
} // namespace SPARQL
