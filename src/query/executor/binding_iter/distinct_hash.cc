#include "distinct_hash.h"

using namespace std;

void DistinctHash::begin(Binding& parent_binding) {
    this->parent_binding = &parent_binding;
    child_iter->begin(parent_binding);
    current_tuple = std::vector<ObjectId>(projected_vars.size());
}


void DistinctHash::reset() {
    child_iter->reset();
    extendable_table.reset();
}


bool DistinctHash::next() {
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


void DistinctHash::assign_nulls() {
    child_iter->assign_nulls();
}


bool DistinctHash::current_tuple_distinct() {
    bool is_new_tuple = !extendable_table.is_in_or_insert(current_tuple);
    return is_new_tuple;
}


void DistinctHash::analyze(std::ostream& os, int indent) const {
    os << std::string(indent, ' ');
    os << "DistinctHash()\n";
    child_iter->analyze(os, indent + 2);
}
