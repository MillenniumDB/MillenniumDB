#pragma once


#include "query/var_id.h"
namespace MQL {

class DisjointVar {
public:
    const VarId var;

    DisjointVar(VarId var) :
        var(var)
    { }

    DisjointVar(const DisjointVar& other) :
        var(other.var)
    { }

    bool operator<(const DisjointVar& other) const
    {
        return var < other.var;
    }

    std::ostream& print(std::ostream& os, int indent = 0) const
    {
        os << std::string(indent, ' ');
        os << "DisjointVar(" << var << ")\n";
        return os;
    }
};
} // namespace MQL
