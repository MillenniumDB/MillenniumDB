#pragma once

#include "execution/binding_id_iter/scan_ranges/scan_range.h"

class Term : public ScanRange {
private:
    ObjectId object_id;

public:
    Term(ObjectId object_id) : object_id(object_id) { }

    uint64_t get_min(BindingId&) override {
        return object_id.id;
    }

    uint64_t get_max(BindingId&) override {
        return object_id.id;
    }

    void try_assign(BindingId&, ObjectId) override { }
};
