#pragma once

#include "query/parser/op/mql/op.h"

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
        os << "DisjointVar(" << get_query_ctx().get_var_name(var) << ")\n";
        return os;
    }
};
} // namespace MQL
