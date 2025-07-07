#pragma once

#include "query/parser/op/gql/op.h"

namespace GQL {

class OpNode : public Op {
public:
    VarId id;

    OpNode(VarId id) :
        id(id)
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
        os << "OpNode: " << id << "\n";
        return os;
    }
};
} // namespace GQL
