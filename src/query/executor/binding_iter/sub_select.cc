#include "sub_select.h"

void SubSelect::_begin(Binding& _parent_binding)
{
    parent_binding = &_parent_binding;

    child_binding = std::make_unique<Binding>(_parent_binding.size);

    for (auto var : safe_assigned_vars) {
        (*child_binding).add(var, (*parent_binding)[var]);
    }

    child_iter->begin(*child_binding);
}

void SubSelect::_reset()
{
    for (auto var : safe_assigned_vars) {
        (*child_binding).add(var, (*parent_binding)[var]);
    }
    child_iter->reset();
}

bool SubSelect::_next()
{
    if (child_iter->next()) {
        for (auto var : projection_vars) {
            (*parent_binding).add(var, (*child_binding)[var]);
        }
        return true;
    } else {
        return false;
    }
}

void SubSelect::assign_nulls()
{
    child_iter->assign_nulls();

    for (auto var : projection_vars) {
        (*parent_binding).add(var, (*child_binding)[var]);
    }
}

void SubSelect::print(std::ostream& os, int indent, bool stats) const
{
    if (stats) {
        print_generic_stats(os, indent);
    }
    os << std::string(indent, ' ') << "SubSelect(";
    auto printed = false;

    if (projection_vars.size() > 0) {
        printed = true;
        os << "proj_vars: ";
        auto first = true;
        for (auto var : projection_vars) {
            if (first)
                first = false;
            else
                os << ", ";
            os << var;
        }
    }

    if (safe_assigned_vars.size() > 0) {
        if (printed)
            os << ", ";
        printed = true;
        os << "fixed_vars: ";
        auto first = true;
        for (auto var : safe_assigned_vars) {
            if (first)
                first = false;
            else
                os << ", ";
            os << var;
        }
    }

    os << ")\n";
    child_iter->print(os, indent + 2, stats);
}
