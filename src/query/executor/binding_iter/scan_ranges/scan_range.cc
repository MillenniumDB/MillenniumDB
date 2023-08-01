#include "scan_range.h"

#include "query/executor/binding_iter/scan_ranges/assigned_var.h"
#include "query/executor/binding_iter/scan_ranges/term.h"
#include "query/executor/binding_iter/scan_ranges/unassigned_var.h"

std::unique_ptr<ScanRange> ScanRange::get(Id id, bool assigned) {
    if (!id.is_var()) {
        return std::make_unique<Term>(id.get_OID());
    } else if (assigned) {
        return std::make_unique<AssignedVar>(id.get_var());
    } else {
        return std::make_unique<UnassignedVar>(id.get_var());
    }
}

std::unique_ptr<ScanRange> ScanRange::get(ObjectId oid) {
    return std::make_unique<Term>(oid);
}
