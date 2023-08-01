#pragma once

#include "query/query_context.h"
#include "query/executor/binding_iter/scan_ranges/scan_range.h"

class UnassignedVar : public ScanRange {
private:
    VarId var;

public:
    UnassignedVar(VarId var) :
        var(var) { }

    uint64_t get_min(Binding&) override {
        return 0;
    }

    uint64_t get_max(Binding&) override {
        return 0xFF'FFFF'FFFFFFFFFFUL;
    }

    void print(std::ostream& os) const override {
        os << var;
    }

    void try_assign(Binding& binding, ObjectId obj_id) override {
        binding.add(var, obj_id);
    }
};
