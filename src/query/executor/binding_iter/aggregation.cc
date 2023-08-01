#include "aggregation.h"

void Aggregation::begin(Binding& _parent_binding) {
    parent_binding = &_parent_binding;
    group_vars_binding = Binding(parent_binding->size);
    child_binding = Binding(parent_binding->size);
    child->begin(child_binding);
    saved_next = child->next();

    for (auto&& [var_id, agg] : aggregations) {
        agg->set_binding(child_binding);
    }
}

void Aggregation::reset() {
    this->child->reset();
}


bool Aggregation::next() {
    if (saved_next) {
        // remember group
        for (auto& var_id : group_vars) {
            group_vars_binding.add(var_id, child_binding[var_id]);
        }
        for (auto&& [var_id, agg] : aggregations) {
            agg->begin();
            agg->process();
        }
    } else {
        if (has_returned || group_vars.size() > 0) {
            return false;
        }
    }

    saved_next = false;
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
            saved_next = true;
            break;
        }
    }
    for (auto&& [var_id, agg] : aggregations) {
        parent_binding->add(var_id, agg->get());
    }
    for (auto & var_id: group_vars) {
        parent_binding->add(var_id, group_vars_binding[var_id]);
    }
    has_returned = true;
    return true;
}

void Aggregation::assign_nulls() {
    for (auto&& [var_id, agg] : aggregations) {
        parent_binding->add(var_id, ObjectId::get_null());
    }
    this->child->assign_nulls();
}

void Aggregation::analyze(std::ostream& os, int indent) const {
    os << std::string(indent, ' ');
    // TODO: print GROUP
    os << "Aggregation(";

    bool first = true;
    for (auto& [var, agg] : aggregations) {
        if (!first) {
            os << ", ";
        } else {
            first = false;
        }

        os << '?' << get_query_ctx().get_var_name(var);
        if (agg) {
            os << '=' << *agg;
        }
    }
    os << ")\n";

    child->analyze(os, indent+2);
}
