#pragma once

#include "query/parser/op/gql/op.h"

namespace GQL {

class OpNode : public Op {
public:
    VarId id;

    // false only for a node that MATCH COLLAPSE adds at an end of an edge pattern
    bool on_path;

    OpNode(VarId id, bool on_path = true) :
        id(id),
        on_path(on_path)
    { }

    bool operator<(const OpNode& other) const
    {
        return id < other.id;
    }

    std::unique_ptr<Op> clone() const override
    {
        return std::make_unique<OpNode>(*this);
    }

    void accept_visitor(OpVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override
    {
        return { id };
    }

    std::map<VarId, GQL::VarType> get_var_types() const override
    {
        std::map<VarId, GQL::VarType> res;
        res[id] = VarType(VarType::Node);
        return res;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override
    {
        os << std::string(indent, ' ');
        os << "OpNode: " << id << (on_path ? "" : " (off-path)") << "\n";
        return os;
    }
};
} // namespace GQL
