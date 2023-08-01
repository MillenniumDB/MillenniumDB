#include "values.h"

void Values::begin(Binding& _parent_binding) {
    parent_binding = &_parent_binding;
    current = 0;
    executions++;
}


void Values::reset() {
    current = 0;
    executions++;
}


bool Values::next() {
while_loop_begin:
    while (current < values.size()) {
        for (unsigned i = 0; i < vars.size(); i++) {
            auto [var, fixed] = vars[i];
            auto parent_oid = (*parent_binding)[var];
            auto values_oid = values[current + i];
            if (fixed and parent_oid != values_oid) {
                current += vars.size();
                goto while_loop_begin;
            }
        }

        for (unsigned i = 0; i < vars.size(); i++) {
            auto [var, fixed] = vars[i];
            parent_binding->add(var, values[current + i]);
        }

        current += vars.size();
        result_count++;
        return true;
    }
    return false;
}


void Values::assign_nulls() {
    for (auto& [var_id, fixed] : vars) {
        parent_binding->add(var_id, ObjectId::get_null());
    }
}


void Values::analyze(std::ostream& os, int indent) const {
    os << std::string(indent, ' ') << "Values(";

    os << "executions:" << executions;
    os << " returned:" << result_count;
    os << " values:" << (values.size() / vars.size());

    os << " vars:";
    auto first = true;
    for (auto& [var, fixed] : vars) {
        if (first) {
            first = false;
        } else {
            os << ",";
        }
        auto fixed_str = fixed? "fixed" : "not-fixed";
        os << "?" << get_query_ctx().get_var_name(var) << "(" << fixed_str << ")";
    }

    os << ")\n";
}
