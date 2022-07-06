#include "scan_range.h"

#include "execution/binding_id_iter/scan_ranges/assigned_var.h"
#include "execution/binding_id_iter/scan_ranges/term.h"
#include "execution/binding_id_iter/scan_ranges/unassigned_var.h"

using namespace std;

unique_ptr<ScanRange> ScanRange::get(Id id, bool assigned) {
    if ( holds_alternative<ObjectId>(id) ) {
        return make_unique<Term>(std::get<ObjectId>(id));
    } else if (assigned) {
        return make_unique<AssignedVar>(std::get<VarId>(id));
    } else {
        return make_unique<UnassignedVar>(std::get<VarId>(id));
    }
}
