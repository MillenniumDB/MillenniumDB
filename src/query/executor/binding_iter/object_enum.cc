#include "object_enum.h"

#include "macros/likely.h"
#include "query/exceptions.h"

void ObjectEnum::_begin(Binding& parent_binding) {
    this->parent_binding = &parent_binding;
    current_node = 0;
}


bool ObjectEnum::_next() {
    if (current_node < max_count) {
        if (MDB_unlikely(get_query_ctx().thread_info.interruption_requested)) {
            throw InterruptedException();
        }
        current_node++;
        parent_binding->add(var, ObjectId(mask | current_node));
        return true;
    } else {
        return false;
    }
}


void ObjectEnum::_reset() {
    current_node = 0;
}


void ObjectEnum::assign_nulls() {
    parent_binding->add(var, ObjectId::get_null());
}


void ObjectEnum::accept_visitor(BindingIterVisitor& visitor) {
    visitor.visit(*this);
}
