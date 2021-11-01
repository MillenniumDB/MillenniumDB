#include "distinct_hash.h"

#include <iostream>

using namespace std;

DistinctHash::DistinctHash(unique_ptr<BindingIter> _child_iter, std::vector<VarId> _projected_vars) :
    child_iter       (move(_child_iter)),
    child_binding    (child_iter->get_binding()),
    projected_vars   (move(_projected_vars)),
    extendable_table (DistinctBindingHash<GraphObject>(projected_vars.size())) { }


void DistinctHash::begin() {
    child_iter->begin();
    current_tuple = std::vector<GraphObject>(projected_vars.size());
}


bool DistinctHash::next() {
    while (child_iter->next()) {
        cout << child_binding << "\n";
        // load current objects
        for (size_t i = 0; i < projected_vars.size(); i++) {
            current_tuple[i] = child_binding[projected_vars[i]];
        }
        if (!extendable_table.is_in_or_insert(current_tuple)) {
            return true;
        }
    }
    return false;
}


void DistinctHash::analyze(std::ostream& os, int indent) const {
    os << std::string(indent, ' ');
    os << "DistinctHash(";
    for (auto& var_id : projected_vars) {
        os << " VarId(" << var_id.id << ")";
    }
    os << " )\n";
    child_iter->analyze(os, indent);
}
