#pragma once

#include "query/executor/binding_iter.h"

class SingleResultBindingIter : public BindingIter {
private:
    bool returned = false;

public:
    void begin(Binding&) override { returned = false; }
    void reset() override { returned = false; }
    void assign_nulls() override { }

    bool next() override {
        if (returned) {
            return false;
        } else {
            returned = true;
            return true;
        }
    }

    void analyze(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ');
        os << "SingleResultBindingIter()\n";
    }
};
