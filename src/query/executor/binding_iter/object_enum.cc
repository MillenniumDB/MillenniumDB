#include "object_enum.h"

#include "macros/likely.h"
#include "query/exceptions.h"

void ObjectEnum::_begin(Binding& parent_binding)
{
    this->parent_binding = &parent_binding;
}

bool ObjectEnum::_next()
{
    if (current_node < max_count) {
        if (MDB_unlikely(get_query_ctx().thread_info.interruption_requested)) {
            throw InterruptedException();
        }
        parent_binding->add(var, ObjectId(mask | current_node));
        current_node++;
        return true;
    } else {
        return false;
    }
}

void ObjectEnum::_reset()
{
    current_node = 0;
}

void ObjectEnum::assign_nulls()
{
    parent_binding->add(var, ObjectId::get_null());
}

void ObjectEnum::print(std::ostream& os, int indent, bool stats) const
{
    if (stats) {
        print_generic_stats(os, indent);
    }
    os << std::string(indent, ' ') << "ObjectEnum(";

    os << "var: " << var;
    os << ", max_count: " << max_count;
    os << ")\n";
}
