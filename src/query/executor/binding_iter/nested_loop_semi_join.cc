#include "nested_loop_semi_join.h"

#include <algorithm>

#include "query/executor/binding_iter/empty_binding_iter.h"

void NestedLoopSemiJoin::begin(Binding& _parent_binding) {
    parent_binding = &_parent_binding;

    lhs_binding = std::make_unique<Binding>(parent_binding->size);
    rhs_binding = std::make_unique<Binding>(parent_binding->size);

    // copy assigned_safe_vars from parent_binding into lhs_binding and rhs_binding
    for (const auto& var : parent_safe_vars) {
        lhs_binding->add(var, (*parent_binding)[var]);
        rhs_binding->add(var, (*parent_binding)[var]);
    }

    lhs->begin(*lhs_binding);
    if (lhs->next()) {
        for (const auto& var : safe_join_vars) {
            rhs_binding->add(var, (*lhs_binding)[var]);
        }
        rhs = original_rhs.get();
    } else {
        rhs = &empty_iter;
    }
    original_rhs->begin(*rhs_binding);
    executions++;
}


bool NestedLoopSemiJoin::next() {
    while (true) {
        if (rhs->next()) {
            bool match = true;
            for (auto& var : unsafe_join_vars) {
                auto lhs_oid = (*lhs_binding)[var];
                if (lhs_oid.is_null()) {
                    continue;
                }

                auto rhs_oid = (*rhs_binding)[var];
                if (rhs_oid.is_null()) {
                    continue;
                }

                if (lhs_oid != rhs_oid) {
                    match = false;
                    break;
                }
            }

            if (match) {
                for (auto& var: lhs_only_vars) {
                    parent_binding->add(var, (*lhs_binding)[var]);
                }

                for (auto& var: safe_join_vars) {
                    parent_binding->add(var, (*rhs_binding)[var]);
                }

                for (auto& var: unsafe_join_vars) {
                    parent_binding->add(var, (*lhs_binding)[var]);
                }
                result_count++;
                return true;
            }
        } else {
            if (lhs->next()) {
                for (const auto& var : safe_join_vars) {
                    rhs_binding->add(var, (*lhs_binding)[var]);
                }
                rhs->reset();
            }
            else {
                return false;
            }
        }
    }
}

void NestedLoopSemiJoin::reset() {
    // copy assigned_safe_vars from parent_binding into lhs_binding and rhs_binding
    for (const auto& var : parent_safe_vars) {
        lhs_binding->add(var, (*parent_binding)[var]);
        rhs_binding->add(var, (*parent_binding)[var]);
    }

    lhs->reset();
    if (lhs->next()) {
        for (const auto& var : safe_join_vars) {
            rhs_binding->add(var, (*lhs_binding)[var]);
        }
        rhs = original_rhs.get();
        rhs->reset();
    } else {
        rhs = &empty_iter;
    }
    executions++;
}


void NestedLoopSemiJoin::assign_nulls() {
    lhs->assign_nulls();
    original_rhs->assign_nulls();
}


void NestedLoopSemiJoin::analyze(std::ostream& os, int indent) const {
    os << std::string(indent, ' ') << "NestedLoopSemiJoin(";

    os << "executions:" << executions;
    os << " found:" << result_count;

    os << "; parent_safe_vars:";
    auto first = true;
    for (auto& var: parent_safe_vars) {
        if (first) {
            first = false;
        } else {
            os << ",";
        }
        os << "?" << get_query_ctx().get_var_name(var);
    }

    os << "; lhs:";
    first = true;
    for (auto& var: lhs_only_vars) {
        if (first) {
            first = false;
        } else {
            os << ",";
        }
        os << "?" << get_query_ctx().get_var_name(var);
    }

    os << "; safe_join_vars:";
    first = true;
    for (auto& var: safe_join_vars) {
        if (first) {
            first = false;
        } else {
            os << ",";
        }
        os << "?" << get_query_ctx().get_var_name(var);
    }

    os << "; unsafe_join_vars:";
    first = true;
    for (auto& var: unsafe_join_vars) {
        if (first) {
            first = false;
        } else {
            os << ",";
        }
        os << "?" << get_query_ctx().get_var_name(var);
    }

    os << "; rhs:";
    first = true;
    for (auto& var: rhs_only_vars) {
        if (first) {
            first = false;
        } else {
            os << ",";
        }
        os << "?" << get_query_ctx().get_var_name(var);
    }

    os << ")\n";
    lhs->analyze(os, indent + 2);
    original_rhs->analyze(os, indent + 2);
}
