#include "object_enum.h"

#include "query/exceptions.h"

void ObjectEnum::begin(Binding& parent_binding) {
    this->parent_binding = &parent_binding;
    current_node = 0;
}


bool ObjectEnum::next() {
    if (current_node < max_count) {
        if (__builtin_expect(!!(get_query_ctx().thread_info.interruption_requested), 0)) {
            throw InterruptedException();
        }
        current_node++;
        parent_binding->add(var, ObjectId(mask | current_node));
        results++;
        return true;
    } else {
        return false;
    }
}


void ObjectEnum::reset() {
    current_node = 0;
}


void ObjectEnum::assign_nulls() {
    parent_binding->add(var, ObjectId::get_null());
}


void ObjectEnum::analyze(std::ostream& os, int indent) const {
    os << std::string(indent, ' ');
    os << "ObjectEnum(results: " << results << ")";
}
