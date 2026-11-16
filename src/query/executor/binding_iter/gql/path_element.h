#pragma once

#include <tuple>

#include "graph_models/object_id.h"

// A node or an edge as it occurs in a path.
//
// A path goes through an edge from one of its endpoints (from) to the other (to). The direction
// relates this to the edge in the graph: DIRECTION_RIGHT if the path follows a directed edge,
// DIRECTION_LEFT if it goes against it and DIRECTION_UNDIRECTED for an undirected edge.
struct PathElement {
    ObjectId id;
    bool is_edge = false;

    // only for edges
    ObjectId direction = ObjectId::get_null();
    ObjectId from = ObjectId::get_null();
    ObjectId to = ObjectId::get_null();

    static PathElement node(ObjectId id)
    {
        return PathElement { id };
    }

    static PathElement edge(ObjectId id, ObjectId direction, ObjectId from, ObjectId to)
    {
        return PathElement { id, true, direction, from, to };
    }

    // The node at which a path that starts with this element starts
    ObjectId start_node() const
    {
        return is_edge ? from : id;
    }

    // The node at which a path that ends with this element ends
    ObjectId end_node() const
    {
        return is_edge ? to : id;
    }

    // The same node, or the same edge traversed in the same direction
    bool operator==(const PathElement& other) const noexcept
    {
        return std::tie(is_edge, id, direction, from, to)
            == std::tie(other.is_edge, other.id, other.direction, other.from, other.to);
    }

    bool operator!=(const PathElement& other) const noexcept
    {
        return !(*this == other);
    }

    // For ordered set
    bool operator<(const PathElement& other) const noexcept
    {
        return std::tie(is_edge, id, direction, from, to)
             < std::tie(other.is_edge, other.id, other.direction, other.from, other.to);
    }
};
