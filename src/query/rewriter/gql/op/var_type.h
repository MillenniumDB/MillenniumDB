#pragma once

#include <memory>

namespace GQL {
struct VarType {
    enum Type {
        Node,
        Edge,
        Path,
    };

    enum Degree {
        Unconditional,
        Conditional,
        Group
    };

    Type type;
    Degree degree;

    VarType(Type type = Node, Degree degree = Unconditional) :
        type(type),
        degree(degree)
    { }

    bool operator==(const VarType& other) const
    {
        return type == other.type && degree == other.degree;
    }

    bool operator!=(const VarType& rhs) const
    {
        return !(*this == rhs);
    }

    inline bool is_singleton()
    {
        return type == Node || type == Edge;
    }

    inline bool is_group()
    {
        return degree == Group;
    }

    inline bool is_conditional()
    {
        return degree == Conditional;
    }

    inline bool is_unconditional()
    {
        return degree == Unconditional;
    }

    inline bool is_path()
    {
        return type == Path;
    }
};

} // namespace GQL
