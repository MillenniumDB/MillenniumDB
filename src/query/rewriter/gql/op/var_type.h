#pragma once

#include <memory>

namespace GQL {
struct VarType {
    enum Type {
        Node,
        Edge,
        Path,
        Group,
        Maybe,
    };

    Type type;

    std::unique_ptr<VarType> child = nullptr;

    VarType(Type type) :
        type(type)
    { }

    bool operator==(const VarType& other) const
    {
        if (type != other.type) {
            return false;
        }
        if (child != nullptr) {
            return *child == *other.child;
        }
        return true;
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
        return type == Group;
    }

    inline bool is_conditional()
    {
        return type == Maybe;
    }

    inline bool is_path()
    {
        return type == Path;
    }
};

struct Node : VarType {
    Node() :
        VarType(Type::Node)
    { }
};

struct Edge : VarType {
    Edge() :
        VarType(Type::Edge)
    { }
};

struct Path : VarType {
    Path() :
        VarType(Type::Path)
    { }
};

struct Group : VarType {
    std::unique_ptr<VarType> child;
    Group(std::unique_ptr<VarType>&& child) :
        VarType(Type::Group)
    {
        VarType::child = std::move(child);
    }
};

struct Maybe : VarType {
    Maybe(std::unique_ptr<VarType>&& child) :
        VarType(Type::Maybe)
    {
        VarType::child = std::move(child);
    }
};

} // namespace GQL
