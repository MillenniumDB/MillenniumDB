#include "object_enum.h"

#include "base/exceptions.h"

using namespace std;

ObjectEnum::ObjectEnum(ThreadInfo* thread_info, VarId var_id, const uint64_t mask, const uint64_t max_count) :
    thread_info (thread_info),
    var_id      (var_id),
    mask        (mask),
    max_count   (max_count) { }


void ObjectEnum::begin(BindingId& parent_binding) {
    this->parent_binding = &parent_binding;
    current_node = 0;
}


bool ObjectEnum::next() {
    if (current_node < max_count) {
        if (__builtin_expect(!!(thread_info->interruption_requested), 0)) {
            throw InterruptedException();
        }
        current_node++;
        parent_binding->add(var_id, ObjectId(mask | current_node));
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
    parent_binding->add(var_id, ObjectId::get_null());
}


void ObjectEnum::analyze(std::ostream& os, int indent) const {
    os << std::string(indent, ' ');
    os << "ObjectEnum(results: " << results << ")";
}
