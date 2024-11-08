#pragma once

#include "query/executor/binding_iter.h"

class SingleResultBindingIter : public BindingIter {
private:
    bool returned = false;

public:
    void _begin(Binding&) override { returned = false; }
    void _reset() override { returned = false;  }
    void assign_nulls() override { }

    bool _next() override {
        if (returned) {
            return false;
        } else {
            returned = true;
            return true;
        }
    }

    void accept_visitor(BindingIterVisitor& visitor) override {
        visitor.visit(*this);
    }
};
