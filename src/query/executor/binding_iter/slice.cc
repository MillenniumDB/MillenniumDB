#include "slice.h"

#include "query/parser/op/sparql/op_select.h"

void Slice::begin(Binding& _parent_binding) {
    parent_binding = &_parent_binding;
    child_id_iter->begin(_parent_binding);
    count = 0;
    position = 0;
    total_count = 0;
}


void Slice::reset() {
    child_id_iter->reset();
    position = 0;
    count = 0;
}


bool Slice::next() {
    while (position < offset) {
        if (child_id_iter->next()) {
            position++;
        } else {
            return false;
        }
    }

    if (count < limit && child_id_iter->next()) {
        count++;
        total_count++;
        return true;
    } else {
        return false;
    }
}


void Slice::assign_nulls() {
    child_id_iter->assign_nulls();
}


void Slice::analyze(std::ostream& os, int indent) const {
    os << std::string(indent, ' ');
    os << "Slice(results: " << total_count;

    if (offset != Op::DEFAULT_OFFSET) {
        os << ", OFFSET " << offset;
    }

    if (limit != Op::DEFAULT_LIMIT) {
        os << ", LIMIT " << limit;
    }
    os << ")\n";
    child_id_iter->analyze(os, indent+2);
}
