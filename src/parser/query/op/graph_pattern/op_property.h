#pragma once

#include <string>

#include "base/query/query_element.h"
#include "parser/query/op/op.h"

class OpProperty : public Op {
public:
    const QueryElement node; // may be a bad variable name, edges also have properties
    const std::string  key;
    const QueryElement value;

    OpProperty(const std::string& node, const std::string& key, const std::string& value) :
        node  (QueryElement::deduce(node)),
        key   (key),
        value (QueryElement::deduce(value)) { }

    OpProperty(const Var& node, const std::string& key, const QueryElement& value) :
        node  (node),
        key   (key),
        value (value) { }

    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }

    // Only comparing node and key, not checking value
    bool operator<(const OpProperty& other) const {
        if (node < other.node) {
            return true;
        } else if (node > other.node) {
            return false;
        } else {
            return key < other.key;
        }
        return false;
    }

    std::set<Var> get_vars() const override {
        std::set<Var> res;
        if (node.is_var()) {
            res.insert(node.to_var());
            res.insert(Var(node.to_var().name + '.' + key));
        }
        // we assume key cant't be a variable
        return res;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ');
        os << "OpProperty(" << node << ", " << key << ", " << value << ")\n";
        return os;
    };
};
