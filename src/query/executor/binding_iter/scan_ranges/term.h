#pragma once

#include "query/query_context.h"
#include "query/executor/binding_iter/scan_ranges/scan_range.h"

class Term : public ScanRange {
private:
    ObjectId object_id;

public:
    Term(ObjectId object_id) : object_id(object_id) { }

    uint64_t get_min(Binding&) override {
        return object_id.id;
    }

    uint64_t get_max(Binding&) override {
        return object_id.id;
    }

    void print(std::ostream& os) const override {
        os << object_id;
    }

    void try_assign(Binding&, ObjectId) override { }
};
