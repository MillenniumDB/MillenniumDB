#include "sub_select.h"

void SubSelect::begin(Binding& parent_binding_) {
    parent_binding = &parent_binding_;

    child_binding = std::make_unique<Binding>(parent_binding_.size);

    for (auto var : safe_assigned_vars) {
        (*child_binding).add(var, (*parent_binding)[var]);
    }

    child_iter->begin(*child_binding);

    executions = 1;
    count      = 0;
}


void SubSelect::reset() {
    for (auto var : safe_assigned_vars) {
        (*child_binding).add(var, (*parent_binding)[var]);
    }

    child_iter->reset();
    executions++;
}


bool SubSelect::next() {
    if (child_iter->next()) {
        count++;

        for (auto var : projection_vars) {
            (*parent_binding).add(var, (*child_binding)[var]);
        }

        return true;
    } else {
        return false;
    }
}


void SubSelect::assign_nulls() {
    child_iter->assign_nulls();

    for (auto var : projection_vars) {
        (*parent_binding).add(var, (*child_binding)[var]);
    }
}


void SubSelect::analyze(std::ostream& os, int indent) const {
    os << std::string(indent, ' ');
    os << "SubSelect(executions: " << executions;
    os << ", count: " << count << "; ";

    os << "proj: ";
    auto first = true;
    for (auto var : projection_vars) {
        if (first) {
            first = false;
        } else {
            os << ", ";
        }

        os << "?" << get_query_ctx().get_var_name(var);
    }

    os << "; safe: ";
    first = true;
    for (auto var : safe_assigned_vars) {
        if (first) {
            first = false;
        } else {
            os << ", ";
        }

        os << "?" << get_query_ctx().get_var_name(var);
    }

    os << ")\n";
    child_iter->analyze(os, indent + 2);
}
