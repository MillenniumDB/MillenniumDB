#pragma once

#include <memory>

#include "query/parser/op/op.h"

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
    EdgeType type;
    std::string type_text;

    OpEdge(VarId from, VarId to, VarId edge, EdgeType type) :
        from(from),
        to(to),
        id(edge),
        type(type)
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
        res[id] = std::make_unique<Edge>();
        return res;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override
    {
        os << std::string(indent, ' ');
        os << "OpEdge (" << type_text << "):" << id << " (" << from << ", " << to << ")\n";
        return os;
    }
};
} // namespace GQL
