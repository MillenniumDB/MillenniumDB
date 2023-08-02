#pragma once

#include <string>

#include "query/id.h"
#include "query/parser/op/op.h"

namespace MQL {

class OpProperty : public Op {
public:
    const Id node;
    const ObjectId key;
    const Id value;

    OpProperty(Id node, ObjectId key, Id value) :
        node  (node),
        key   (key),
        value (value) { }

    virtual std::unique_ptr<Op> clone() const override {
        return std::make_unique<OpProperty>(node, key, value);
    }

    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }

    // Only comparing node and key, not checking value
    bool operator<(const OpProperty& other) const {
        if (node < other.node) {
            return true;
        } else if (other.node < node) {
            return false;
        } else if (key < other.key) {
            return true;
        } else if (key > other.key) {
            return false;
        } else {
            return value < other.value;
        }
    }

    std::set<VarId> get_all_vars() const override {
        std::set<VarId> res;
        if (node.is_var()) {
            res.insert(node.get_var());
        }
        // we assume key cant't be a variable
        if (value.is_var()) {
            res.insert(value.get_var());
        }
        return res;
    }

    std::set<VarId> get_scope_vars() const override {
        return get_all_vars();
    }

    std::set<VarId> get_safe_vars() const override {
        return get_all_vars();
    }

    std::set<VarId> get_fixable_vars() const override {
        return get_all_vars();
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ');
        os << "OpProperty("
           << node
           << ", " << key
           << ", " << value
           << ")\n";
        return os;
    }
};
} // namespace MQL
