#pragma once

#include "query/executor/binding_iter.h"

class EmptyBindingIter : public BindingIter {
public:
    void _begin(Binding&) override { }

    bool _next() override
    {
        return false;
    }

    void _reset() override { }

    void assign_nulls() override { }

    void print(std::ostream& os, int indent, bool) const override
    {
        os << std::string(indent, ' ') << "EmptyBindingIter()\n";
    }
};
