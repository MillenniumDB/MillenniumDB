#pragma once

#include <string>

#include "base/query/query_element.h"
#include "parser/query/op/op.h"

class OpLabel : public Op {
public:
    const QueryElement node;

    const std::string label;

    OpLabel(const std::string& node, const std::string& label) :
        node  (QueryElement::deduce(node)),
        label (label.substr(1, label.size() - 1)) // remove leading ':'
        { }

    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }

    bool operator<(const OpLabel& other) const {
        if (node < other.node) {
            return true;
        } else if (node > other.node) {
            return false;
        } else {
            return label < other.label;
        }
    }

    std::set<Var> get_vars() const override {
        std::set<Var> res;
        if (node.is_var()) {
            res.insert(node.to_var());
        }
        // we assume label won't be a variable
        return res;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ');
        os << "OpLabel(" << node << ", " << label << ")\n";
        return os;
    }
};
