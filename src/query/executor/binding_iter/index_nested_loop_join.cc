#include "index_nested_loop_join.h"

#include <algorithm>

#include "query/var_id.h"
#include "query/executor/binding_iter/empty_binding_iter.h"


void IndexNestedLoopJoin::begin(Binding& parent_binding) {
    this->parent_binding = &parent_binding;

    lhs->begin(parent_binding);
    if (lhs->next()) {
        rhs = original_rhs.get();
    } else {
        rhs = &empty_iter;
    }
    original_rhs->begin(parent_binding);
    executions++;
}

bool IndexNestedLoopJoin::next() {
    while (true) {
        if (rhs->next()) {
            result_count++;
            return true;
        } else {
            if (lhs->next())
                rhs->reset();
            else
                return false;
        }
    }
}

void IndexNestedLoopJoin::reset() {
    lhs->reset();
    if (lhs->next()) {
        rhs = original_rhs.get();
        rhs->reset();
    } else {
        rhs = &empty_iter;
    }
    executions++;
}


void IndexNestedLoopJoin::assign_nulls() {
    lhs->assign_nulls();
    original_rhs->assign_nulls();
}


void IndexNestedLoopJoin::analyze(std::ostream& os, int indent) const {
    os << std::string(indent, ' ') << "IndexNestedLoopJoin(";

    os << "executions:" << executions;
    os << " found:" << result_count;
    os << ")\n";

    lhs->analyze(os, indent + 2);
    original_rhs->analyze(os, indent + 2);
}

template class std::unique_ptr<IndexNestedLoopJoin>;
