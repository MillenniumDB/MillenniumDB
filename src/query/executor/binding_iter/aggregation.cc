#include "aggregation.h"

void Aggregation::_begin(Binding& _parent_binding) {
    parent_binding = &_parent_binding;

    group_vars_binding = Binding(parent_binding->size);
    child_binding = Binding(parent_binding->size);

    child->begin(child_binding);
    new_group = child->next();

    for (auto&& [var_id, agg] : aggregations) {
        agg->set_binding(child_binding);
        agg->begin();
    }
}


void Aggregation::_reset() {
    this->child->reset();
}


bool Aggregation::_next() {
    if (new_group) {
        // remember group
        for (auto& var_id : group_vars) {
            group_vars_binding.add(var_id, child_binding[var_id]);
        }
        for (auto&& [var_id, agg] : aggregations) {
            agg->begin();
            agg->process();
        }
    } else {
        if (groups == 0 && group_vars.size() == 0) {
            // We have aggregation without grouping and have never returned a binding.
            // But for aggregation without grouping we have to return at least once.
            for (auto&& [var_id, agg] : aggregations) {
                parent_binding->add(var_id, agg->get());
            }
            groups++;
            return true;
        } else {
            return false;
        }
    }

    new_group = false;
    while (child->next()) {
        bool same_group = true;
        // check if group is changed
        for (auto& var_id : group_vars) {
            if ( child_binding[var_id] != group_vars_binding[var_id] ) {
                same_group = false;
            }
        }
        if (same_group) {
            for (auto&& [var_id, agg] : aggregations) {
                agg->process();
            }
        } else {
            new_group = true;
            break;
        }
    }
    for (auto&& [var_id, agg] : aggregations) {
        parent_binding->add(var_id, agg->get());
    }
    for (auto& var_id: group_vars) {
        parent_binding->add(var_id, group_vars_binding[var_id]);
    }
    groups++;
    return true;
}


void Aggregation::assign_nulls() {
    for (auto&& [var_id, agg] : aggregations) {
        parent_binding->add(var_id, ObjectId::get_null());
    }
    child->assign_nulls();
}


void Aggregation::accept_visitor(BindingIterVisitor& visitor) {
    visitor.visit(*this);
}
