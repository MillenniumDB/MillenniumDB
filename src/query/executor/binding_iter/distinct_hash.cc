#include "distinct_hash.h"

using namespace std;

void DistinctHash::_begin(Binding& parent_binding) {
    this->parent_binding = &parent_binding;
    child_iter->begin(parent_binding);
    current_tuple = std::vector<ObjectId>(projected_vars.size());
}


void DistinctHash::_reset() {
    child_iter->reset();
    extendable_table.reset();
}


bool DistinctHash::_next() {
    while (child_iter->next()) {
        processed++;
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


void DistinctHash::accept_visitor(BindingIterVisitor& visitor) {
    visitor.visit(*this);
}
