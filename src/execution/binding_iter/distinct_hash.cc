#include "distinct_hash.h"

using namespace std;

DistinctHash::DistinctHash(unique_ptr<BindingIter> _child_iter,
                           vector<VarId> _projected_vars) :
    child_iter       (move(_child_iter)),
    projected_vars   (move(_projected_vars)),
    extendable_table (DistinctBindingHash<GraphObject>(projected_vars.size())) { }


void DistinctHash::begin(std::ostream& os) {
    child_iter->begin(os);
    current_tuple = std::vector<GraphObject>(projected_vars.size());
}


bool DistinctHash::next() {
    while (child_iter->next()) {
        // load current objects
        for (size_t i = 0; i < projected_vars.size(); i++) {
            current_tuple[i] = (*child_iter)[projected_vars[i]];
        }
        if (!extendable_table.is_in_or_insert(current_tuple)) {
            return true;
        }
    }
    return false;
}


GraphObject DistinctHash::operator[](VarId var_id) const {
    return (*child_iter)[var_id];
}


void DistinctHash::analyze(std::ostream& os, int indent) const {
    os << std::string(indent, ' ');
    child_iter->analyze(os, indent);
    os << "DistinctHash(";
    for (auto& var_id : projected_vars) {
        os << " VarId(" << var_id.id << ")";
    }
    os << " )\n";
}
