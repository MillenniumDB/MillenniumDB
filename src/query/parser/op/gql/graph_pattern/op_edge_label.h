#pragma once

#include "query/parser/op/gql/op.h"

namespace GQL {
class OpEdgeLabel : public Op {
public:
    VarId edge_id;
    ObjectId label_id;

    OpEdgeLabel(VarId edge_id, ObjectId label_id) :
        edge_id(edge_id),
        label_id(label_id)
    { }

    std::unique_ptr<Op> clone() const override
    {
        return std::make_unique<OpEdgeLabel>(edge_id, label_id);
    }

    void accept_visitor(OpVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override
    {
        return { edge_id };
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override
    {
        os << std::string(indent, ' ');
        os << "OpEdgeLabel(" << edge_id << ", " << label_id << ")\n";
        return os;
    }
};
} // namespace GQL
