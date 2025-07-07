#include "slice.h"

void Slice::_begin(Binding& _parent_binding)
{
    parent_binding = &_parent_binding;
    child_iter->begin(_parent_binding);
    count = 0;
    position = 0;
}

void Slice::_reset()
{
    child_iter->reset();
    position = 0;
    count = 0;
}

bool Slice::_next()
{
    while (position < offset) {
        if (child_iter->next()) {
            position++;
        } else {
            return false;
        }
    }

    if (count < limit && child_iter->next()) {
        count++;
        return true;
    } else {
        return false;
    }
}

void Slice::assign_nulls()
{
    child_iter->assign_nulls();
}

void Slice::print(std::ostream& os, int indent, bool stats) const
{
    if (stats) {
        print_generic_stats(os, indent);
    }
    os << std::string(indent, ' ') << "Slice(";
    auto printed = false;
    if (offset != 0) {
        printed = true;
        os << "offset: " << offset;
    }

    if (limit != UINT64_MAX) {
        if (printed)
            os << ", ";
        printed = true;
        os << "limit: " << limit;
    }

    os << ")\n";
    child_iter->print(os, indent + 2, stats);
}
