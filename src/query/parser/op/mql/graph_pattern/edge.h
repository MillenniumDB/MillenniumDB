#pragma once

#include "query/id.h"

namespace MQL {

class Edge {
public:
    const Id from;
    const Id to;
    const Id type;
    const Id edge;

    Edge(Id from, Id to, Id type, Id edge) :
        from(from),
        to(to),
        type(type),
        edge(edge)
    { }

    bool operator<(const Edge& other) const
    {
        if (from < other.from) {
            return true;
        } else if (other.from < from) {
            return false;
        } else if (to < other.to) {
            return true;
        } else if (other.to < to) {
            return false;
        } else if (edge < other.edge) {
            return true;
        } else if (other.edge < edge) {
            return false;
        } else {
            return type < other.type;
        }
    }

    std::ostream& print(std::ostream& os, int indent = 0) const
    {
        return os << std::string(indent, ' ') << "Edge(" << from << ", " << to << ", " << type << ", " << edge
                  << ")\n";
    }
};
} // namespace MQL
