#include "values.h"

void Values::_begin(Binding& _parent_binding)
{
    parent_binding = &_parent_binding;
    current = 0;
}

void Values::_reset()
{
    current = 0;
}

bool Values::_next()
{
while_loop_begin:
    while (current < values.size()) {
        for (unsigned i = 0; i < vars.size(); i++) {
            auto [var, fixed] = vars[i];
            auto parent_oid = (*parent_binding)[var];
            auto values_oid = values[current + i];
            if (fixed && parent_oid != values_oid) {
                current += vars.size();
                goto while_loop_begin;
            }
        }

        for (unsigned i = 0; i < vars.size(); i++) {
            auto [var, fixed] = vars[i];
            parent_binding->add(var, values[current + i]);
        }

        current += vars.size();
        return true;
    }
    return false;
}

void Values::assign_nulls()
{
    for (auto& [var_id, fixed] : vars) {
        parent_binding->add(var_id, ObjectId::get_null());
    }
}

void Values::print(std::ostream& os, int indent, bool stats) const
{
    if (stats) {
        print_generic_stats(os, indent);
    }
    os << std::string(indent, ' ') << "Values()\n";

    os << "vars: ";

    auto first = true;
    for (auto& [var, fixed] : vars) {
        if (first)
            first = false;
        else
            os << ", ";
        if (fixed)
            os << "[";
        os << "?" << get_query_ctx().get_var_name(var);
        if (fixed)
            os << "]";
    }

    os << "\n";

    for (size_t i = 0; i < values.size(); i += vars.size()) {
        os << std::string(indent, ' ') << '(';
        for (size_t j = 0; j < vars.size(); j++) {
            if (j != 0)
                os << ", ";
            os << values[i + j];
        }
        os << ")\n";
    }

    os << std::string(indent - 2, ' ') << ")\n";
}
