#pragma once

#include "query/executor/binding_iter.h"

class EmptyBindingIter : public BindingIter {
public:
    void accept_visitor(BindingIterVisitor& visitor) override {
        visitor.visit(*this);
    }

    void _begin(Binding&) override { }
    bool _next() override { return false; }
    void _reset() override { }
    void assign_nulls() override { }
};
