#pragma once

#include <memory>

#include "query/parser/op/op.h"

namespace GQL {

struct LabelOpId {
    std::unique_ptr<Op> op;
    VarId id;

    LabelOpId(std::unique_ptr<Op> op, VarId id) :
        op(std::move(op)),
        id(id)
    { }
};

class OpNodeLabel : public Op {
public:
    VarId node_id;
    ObjectId label_id;

    OpNodeLabel(VarId node_id, ObjectId label_id) :
        node_id(node_id),
        label_id(label_id)
    { }

    std::unique_ptr<Op> clone() const override
    {
        return std::make_unique<OpNodeLabel>(node_id, label_id);
    }

    void accept_visitor(OpVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override
    {
        return { node_id };
    }

    std::set<VarId> get_scope_vars() const override
    {
        return get_all_vars();
    }

    std::set<VarId> get_safe_vars() const override
    {
        return { node_id };
    }

    std::set<VarId> get_fixable_vars() const override
    {
        return get_all_vars();
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override
    {
        os << std::string(indent, ' ');
        os << "OpNodeLabel(" << node_id << ", " << label_id << ")\n";
        return os;
    }
};
} // namespace GQL
