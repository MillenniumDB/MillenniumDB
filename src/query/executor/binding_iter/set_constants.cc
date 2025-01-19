#include "set_constants.h"

#include "graph_models/rdf_model/conversions.h"

void SetConstants::_begin(Binding& _parent_binding)
{
    parent_binding = &_parent_binding;
    for (auto& [var, oid]: var2oid) {
        parent_binding->add(var, oid);
    }
    child_iter->begin(_parent_binding);
}

bool SetConstants::_next()
{
    return child_iter->next();
}

void SetConstants::_reset()
{
    for (auto& [var, oid]: var2oid) {
        parent_binding->add(var, oid);
    }
    child_iter->reset();
}

void SetConstants::assign_nulls()
{
    child_iter->assign_nulls();
}

void SetConstants::accept_visitor(BindingIterVisitor& visitor)
{
    visitor.visit(*this);
}
