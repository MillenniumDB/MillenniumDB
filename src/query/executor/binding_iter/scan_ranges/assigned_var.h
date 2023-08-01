#pragma once

#include <cassert>

#include "query/query_context.h"
#include "query/executor/binding_iter/scan_ranges/scan_range.h"

class AssignedVar : public ScanRange {
private:
    VarId var;

public:
    AssignedVar(VarId var) : var(var) { }

    uint64_t get_min(Binding& binding) override {
        return binding[var].id;
    }

    uint64_t get_max(Binding& binding) override {
        return binding[var].id;
    }

    void print(std::ostream& os) const override {
        os << '[' << var << ']';
    }

    void try_assign(Binding&, ObjectId) override { }
};
