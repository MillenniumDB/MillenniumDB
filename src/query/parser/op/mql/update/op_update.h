#pragma once

#include <algorithm>
#include <cassert>
#include <memory>
#include <vector>

#include "misc/set_operations.h"
#include "query/parser/op/op.h"

namespace MQL {
class OpUpdate : public Op {
public:
    std::vector<std::unique_ptr<Op>> updates;

    OpUpdate() = default;

    OpUpdate(std::vector<std::unique_ptr<Op>>&& updates) :
        updates (std::move(updates)) { }

    void operator=(OpUpdate&& other) {
        updates = std::move(other.updates);
    }

    std::unique_ptr<Op> clone() const override {
        std::vector<std::unique_ptr<Op>> new_updates;
        new_updates.reserve(updates.size());
        for (auto& op : updates) {
            new_updates.push_back(op->clone());
        }
        return std::make_unique<OpUpdate>(std::move(new_updates));
    }

    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override {
        std::set<VarId> res;
        for (auto& child : updates) {
            for (auto& child_var : child->get_all_vars()) {
                res.insert(child_var);
            }
        }
        return res;
    }

    std::set<VarId> get_scope_vars() const override {
        std::set<VarId> res;
        // for (auto& child : updates) {
        //     for (auto& child_var : child->get_scope_vars()) {
        //         res.insert(child_var);
        //     }
        // }
        return res;
    }

    std::set<VarId> get_safe_vars() const override {
        std::set<VarId> res;
        // for (auto& child : updates) {
        //     for (auto& child_var : child->get_safe_vars()) {
        //         res.insert(child_var);
        //     }
        // }
        return res;
    }

    std::set<VarId> get_fixable_vars() const override {
        std::set<VarId> res;
        // for (auto& child : updates) {
        //     for (auto& child_var : child->get_fixable_vars()) {
        //         res.insert(child_var);
        //     }
        // }
        return res;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ') << "OpUpdate()\n";

        for (auto& child : updates) {
            child->print_to_ostream(os, indent + 2);
        }

        return os;
    }
};
} // namespace MQL
