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


void Values::accept_visitor(BindingIterVisitor& visitor)
{
    visitor.visit(*this);
}
