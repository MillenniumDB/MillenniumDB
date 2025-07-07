#pragma once

#include "query/parser/op/gql/op.h"

namespace GQL {

class OpEdge : public Op {
public:
    enum EdgeType {
        Directed,
        Undirected,
        UndirectedOrDirectedTo,
        DirectedLeftOrRight,
        AnyDirection,
    };

    VarId from;
    VarId to;
    VarId id;
    VarId direction_var;
    EdgeType type;
    std::string type_text;

    // Represents the direction in the query: LEFT, RIGHT OR UNDIRECTED
    std::optional<ObjectId> direction;

    OpEdge(
        VarId from,
        VarId to,
        VarId edge,
        VarId direction_var,
        EdgeType type,
        std::optional<ObjectId> direction = std::nullopt
    ) :
        from(from),
        to(to),
        id(edge),
        direction_var(direction_var),
        type(type),
        direction(direction)
    {
        switch (type) {
        case Directed:
            type_text = "Directed";
            break;
        case Undirected:
            type_text = "Undirected";
            break;
        case UndirectedOrDirectedTo:
            type_text = "UndirectedOrDirectedTo";
            break;
        case DirectedLeftOrRight:
            type_text = "DirectedLeftOrRight";
            break;
        case AnyDirection:
            type_text = "AnyDirection";
            break;
        }
    }

    bool operator<(const OpEdge& other) const
    {
        return id < other.id;
    }

    std::unique_ptr<Op> clone() const override
    {
        return std::make_unique<OpEdge>(*this);
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
        std::map<VarId, GQL::VarType> result;
        result[id] = VarType(VarType::Edge);
        return result;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override
    {
        os << std::string(indent, ' ');
        os << "OpEdge (" << type_text << "):" << id << " (" << from << ", " << to << ")\n";
        return os;
    }
};
} // namespace GQL
