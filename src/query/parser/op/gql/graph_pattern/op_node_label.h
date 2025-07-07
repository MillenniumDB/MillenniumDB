#pragma once

#include "query/parser/op/gql/op.h"

namespace GQL {

struct LabelOpId {
    std::unique_ptr<Op> op;
    VarId id;
    ObjectId label_id;

    LabelOpId(std::unique_ptr<Op> op, VarId id, ObjectId label_id) :
        op(std::move(op)),
        id(id),
        label_id(label_id)
    { }
    LabelOpId(const LabelOpId&) = delete;
    LabelOpId(LabelOpId&&) = default;
    ~LabelOpId() = default;
    LabelOpId& operator=(LabelOpId&&) = default;

    bool operator<(const LabelOpId& other) const
    {
        if (id.id != other.id.id) {
            return id.id < other.id.id;
        }
        return label_id < other.label_id;
    }
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

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override
    {
        os << std::string(indent, ' ');
        os << "OpNodeLabel(" << node_id << ", " << label_id << ")\n";
        return os;
    }
};
} // namespace GQL
