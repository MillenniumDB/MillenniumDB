#pragma once

#include "graph_models/object_id.h"
#include "query/parser/op/mql/op.h"

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
        os << std::string(indent, ' ');
        os << "DisjointTerm(";
        os << term;
        os << ")\n";
        return os;
    }
};
} // namespace MQL
