#pragma once

#include <memory>
#include <vector>

#include "query/parser/op/op.h"

namespace MQL {

class OpOptional : public Op {
public:
    std::unique_ptr<Op> op;

    std::vector<std::unique_ptr<Op>> optionals;

    OpOptional(
        std::unique_ptr<Op> op,
        std::vector<std::unique_ptr<Op>>&& optionals
    ) :
        op(std::move(op)),
        optionals(std::move(optionals)) { }

    std::unique_ptr<Op> clone() const override {
        std::vector<std::unique_ptr<Op>> optionals_clone;
        optionals_clone.reserve(optionals.size());
        for (auto& optional : optionals) {
            optionals_clone.push_back(optional->clone());
        }
        return std::make_unique<OpOptional>(op->clone(), std::move(optionals_clone));
    }

    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override {
        auto res = op->get_all_vars();
        for (auto& child : optionals) {
            for (auto& child_var : child->get_all_vars()) {
                res.insert(child_var);
            }
        }
        return res;
    }

    std::set<VarId> get_scope_vars() const override {
        auto res = op->get_scope_vars();
        for (auto& child : optionals) {
            for (auto& child_var : child->get_scope_vars()) {
                res.insert(child_var);
            }
        }
        return res;
    }

    std::set<VarId> get_safe_vars() const override {
        return op->get_safe_vars();
    }

    std::set<VarId> get_fixable_vars() const override {
        return op->get_fixable_vars();
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ');
        os << "OpOptional()\n";
        os << std::string(indent + 2, ' ') << "main pattern:\n";
        op->print_to_ostream(os, indent + 4);
        os << std::string(indent + 2, ' ') << "children:\n";
        for (auto& optional : optionals) {
            optional->print_to_ostream(os, indent + 4);
        }
        return os;
    }
};
} // namespace MQL
