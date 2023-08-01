#pragma once

#include "query/executor/binding_iter.h"

class EmptyBindingIter : public BindingIter {
public:
    void analyze(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ');
        os << "EmptyBindingIter()";
    }

    void begin(Binding&) override { }
    bool next() override { return false; }
    void reset() override { }
    void assign_nulls() override { }
};
