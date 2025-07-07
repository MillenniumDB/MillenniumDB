#pragma once

#include "query/id.h"
#include "query/parser/op/mql/op.h"

namespace MQL {

class OpLabel : public Op {
public:
    Id node;

    Id label;

    OpLabel(Id node, Id label) :
        node(node),
        label(label)
    { }

    std::unique_ptr<Op> clone() const override
    {
        return std::make_unique<OpLabel>(*this);
    }

    void accept_visitor(OpVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    bool operator<(const OpLabel& other) const
    {
        if (node < other.node) {
            return true;
        } else if (other.node < node) {
            return false;
        } else {
            return label < other.label;
        }
    }

    std::set<VarId> get_all_vars() const override
    {
        std::set<VarId> res;
        if (node.is_var()) {
            res.insert(node.get_var());
        }
        // we assume label won't be a variable
        return res;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override
    {
        os << std::string(indent, ' ');
        os << "OpLabel(";
        os << node;
        os << ", ";
        os << label;
        os << ")\n";
        return os;
    }
};
} // namespace MQL
