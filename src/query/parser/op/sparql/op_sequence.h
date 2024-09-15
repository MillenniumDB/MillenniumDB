#pragma once

#include <vector>

#include "query/parser/op/op.h"

namespace SPARQL {

class OpSequence : public Op {
public:
    std::vector<std::unique_ptr<Op>> ops; // operators to join

    OpSequence(std::vector<std::unique_ptr<Op>>&& ops) :
        ops (std::move(ops)) { }

    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::unique_ptr<Op> clone() const  override {
        std::vector<std::unique_ptr<Op>> new_ops;
        new_ops.reserve(ops.size());
        for (auto& op : ops) {
            new_ops.push_back(op->clone());
        }
        return std::make_unique<OpSequence>(std::move(new_ops));
    }

    std::set<VarId> get_all_vars() const override {
        std::set<VarId> res;
        for (auto& op: ops) {
            auto op_vars = op->get_all_vars();
            res.insert(op_vars.begin(), op_vars.end());
        }
        return res;
    }

    std::set<VarId> get_scope_vars() const override {
        std::set<VarId> res;
        for (auto& op: ops) {
            auto op_vars = op->get_scope_vars();
            res.insert(op_vars.begin(), op_vars.end());
        }
        return res;
    }

    std::set<VarId> get_safe_vars() const override {
        std::set<VarId> res;
        for (auto& op: ops) {
            auto op_vars = op->get_safe_vars();
            res.insert(op_vars.begin(), op_vars.end());
        }
        return res;
    }

    std::set<VarId> get_fixable_vars() const override {
        std::set<VarId> res;
        for (auto& op: ops) {
            auto op_vars = op->get_fixable_vars();
            res.insert(op_vars.begin(), op_vars.end());
        }
        return res;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ') << "OpSequence()\n";

        for (auto& op : ops) {
            op->print_to_ostream(os, indent + 2);
        }

        return os;
    }
};
} // namespace SPARQL
