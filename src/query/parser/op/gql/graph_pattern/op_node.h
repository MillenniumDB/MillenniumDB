#pragma once

#include <memory>

#include "query/parser/op/op.h"
#include "query/rewriter/gql/op/var_type.h"

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

    std::set<VarId> get_scope_vars() const override
    {
        return get_all_vars();
    }

    std::set<VarId> get_safe_vars() const override
    {
        return { id };
    }

    std::set<VarId> get_fixable_vars() const override
    {
        return get_all_vars();
    }

    std::map<VarId, std::unique_ptr<GQL::VarType>> get_var_types() const override
    {
        std::map<VarId, std::unique_ptr<GQL::VarType>> res;
        res[id] = std::make_unique<Node>();
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
