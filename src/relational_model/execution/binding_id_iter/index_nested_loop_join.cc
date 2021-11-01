#include "index_nested_loop_join.h"

#include <algorithm>

#include "base/ids/var_id.h"
#include "relational_model/execution/binding_id_iter/empty_binding_id_iter.h"

using namespace std;

IndexNestedLoopJoin::IndexNestedLoopJoin(unique_ptr<BindingIdIter> _lhs,
                                         unique_ptr<BindingIdIter> _rhs) :
    lhs          (move(_lhs)),
    original_rhs (move(_rhs)) { }


void IndexNestedLoopJoin::begin(BindingId& parent_binding) {
    this->parent_binding = &parent_binding;

    lhs->begin(parent_binding);
    if (lhs->next()) {
        rhs = original_rhs.get();
    } else {
        rhs = &EmptyBindingIdIter::instance;
    }
    original_rhs->begin(parent_binding);
}

bool IndexNestedLoopJoin::next() {
    while (true) {
        if (rhs->next()) {
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
        rhs = &EmptyBindingIdIter::instance;
    }
}


void IndexNestedLoopJoin::assign_nulls() {
    lhs->assign_nulls();
    original_rhs->assign_nulls();
}


void IndexNestedLoopJoin::analyze(std::ostream& os, int indent) const {
    os << std::string(indent, ' ');
    os << "IndexNestedLoopJoin(\n";
    lhs->analyze(os, indent + 2);
    os << ",\n";
    original_rhs->analyze(os, indent + 2);
    os << "\n";
    os << std::string(indent, ' ');
    os << ")";
}

template class std::unique_ptr<IndexNestedLoopJoin>;
