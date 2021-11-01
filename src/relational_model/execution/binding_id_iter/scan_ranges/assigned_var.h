#ifndef RELATIONAL_MODEL__ASSIGNED_VAR_H_
#define RELATIONAL_MODEL__ASSIGNED_VAR_H_

#include <cassert>

#include "relational_model/execution/binding_id_iter/scan_ranges/scan_range.h"

class AssignedVar : public ScanRange {
private:
    VarId var_id;

public:
    AssignedVar(VarId var_id) :
        var_id(var_id) { }

    uint64_t get_min(BindingId& binding_id) override {
        return binding_id[var_id].id;
    }

    uint64_t get_max(BindingId& binding_id) override {
        return binding_id[var_id].id;
    }

    void try_assign(BindingId&, ObjectId) override { }
};

#endif // RELATIONAL_MODEL__ASSIGNED_VAR_H_
