#pragma once

#include "graph_models/object_id.h"

namespace MQL {

class DisjointTerm {
public:
    const ObjectId term;

    DisjointTerm(ObjectId term) :
        term(term)
    { }

    bool operator<(const DisjointTerm& other) const
    {
        return term < other.term;
    }

    std::ostream& print(std::ostream& os, int indent = 0) const
    {
        return os << std::string(indent, ' ') << "DisjointTerm(" << term << ")\n";
    }
};
} // namespace MQL
