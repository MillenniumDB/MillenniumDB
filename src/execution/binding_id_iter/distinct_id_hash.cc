#include "distinct_id_hash.h"

using namespace std;

DistinctIdHash::DistinctIdHash(unique_ptr<BindingIdIter> _child_iter, std::vector<VarId> _projected_vars) :
    child_iter       (move(_child_iter)),
    projected_vars   (move(_projected_vars)),
    extendable_table (DistinctBindingHash<ObjectId>( projected_vars.size() )) { }


void DistinctIdHash::begin(BindingId& parent_binding) {
    this->parent_binding = &parent_binding;
    child_iter->begin(parent_binding);
    current_tuple = std::vector<ObjectId>(projected_vars.size());
}


void DistinctIdHash::reset() {
    child_iter->reset();
    // TODO: now this method is never called, maybe in the future we may need to clear hash table
    // extendable_table.reset();
}


bool DistinctIdHash::next() {
    while (child_iter->next()) {
        // load current objects
        for (size_t i = 0; i < projected_vars.size(); i++) {
            current_tuple[i] = (*parent_binding)[projected_vars[i]];
        }
        if (current_tuple_distinct()) {
            return true;
        }
    }
    return false;
}


void DistinctIdHash::assign_nulls() {
    child_iter->assign_nulls();
}


bool DistinctIdHash::current_tuple_distinct() {
    bool is_new_tuple = !extendable_table.is_in_or_insert(current_tuple);
    return is_new_tuple;
}


void DistinctIdHash::analyze(std::ostream& os, int indent) const {
    child_iter->analyze(os, indent);
    os << "\n";
    os << std::string(indent, ' ');
    os << "DistinctIdHash()";
}
