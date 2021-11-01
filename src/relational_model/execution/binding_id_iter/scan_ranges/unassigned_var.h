#ifndef RELATIONAL_MODEL__UNASSIGNED_VAR_H_
#define RELATIONAL_MODEL__UNASSIGNED_VAR_H_

#include "relational_model/execution/binding_id_iter/scan_ranges/scan_range.h"

class UnassignedVar : public ScanRange {
private:
    VarId var_id;

public:
    UnassignedVar(VarId var_id) :
        var_id(var_id) { }

    uint64_t get_min(BindingId&) override {
        return 0;
    }

    uint64_t get_max(BindingId&) override {
        return 0xFF'FFFF'FFFFFFFFFFUL;
    }

    void try_assign(BindingId& binding, ObjectId obj_id) override {
        binding.add(var_id, obj_id);
    }
};

#endif // RELATIONAL_MODEL__UNASSIGNED_VAR_H_
